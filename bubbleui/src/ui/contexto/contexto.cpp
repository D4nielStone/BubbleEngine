// Copyright (c) 2024 Daniel Oliveira

#include <glad/glad.h>
#include "contexto.hpp"
#include <shlobj.h> // Necessário para SHGetKnownFolderPath
#include <map>
#include <future>
#include <thread>
#include <comdef.h> // Necessário para _bstr_t (conversão de wchar_t para string)
#include "src/arquivadores/imageloader.hpp"
#include "src/ui/painel/painel.hpp"
#include "src/ui/painel/visualizador_de_projetos.hpp"
#include <atomic>  // Para o uso de variáveis atômicas

// contextos existentes
std::map<GLFWwindow*, std::shared_ptr<BubbleUI::Contexto>> contextos;

// Contexto padrão
BubbleUI::Contexto* contexto_atual{ nullptr };
std::condition_variable contexto_pronto;
std::mutex mtx;

bool BubbleUI::parar()
{
    return (contextos.size() == 0);
}

static std::string obterDiretorioDoc()
{
    // Ponteiro para armazenar o caminho
    PWSTR caminhoDocumentos = NULL;
    std::string caminhoDocumentosStr;

    // Obtém o caminho da pasta Documentos (FOLDERID_Documents)
    HRESULT resultado = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &caminhoDocumentos);

    if (SUCCEEDED(resultado)) {
        // Converte o caminho de wchar_t* para string (opcional)
        _bstr_t bstrPath(caminhoDocumentos);
        caminhoDocumentosStr = bstrPath;
    }
    else {
    }

    // Libera a memória alocada
    if (caminhoDocumentos) {
        CoTaskMemFree(caminhoDocumentos);
    }
    return caminhoDocumentosStr;
}

BubbleUI::Contexto::Contexto(GLFWwindow* window)
{

    cursor_horizontal = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursor_vertical = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    cursor_normal = glfwCreateStandardCursor(GLFW_CURSOR_NORMAL);
    cursor_texto = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursor = cursor_normal;
    dirDoProjeto = obterDiretorioDoc() + "/Bubble Engine/Projetos";
    if (window != nullptr) definirJanela(window);
}

BubbleUI::Contexto::~Contexto() = default;

static void callback_size(GLFWwindow* janela, int width, int height)
{
    contexto_atual->tamanho.height = height;
    contexto_atual->tamanho.width = width;
}

void BubbleUI::Contexto::definirJanela(GLFWwindow* janela)
{
    glfwWindow = janela;
    inputs = std::make_shared<Bubble::Inputs::Inputs>();
    contexto_atual = this;

    // definir inputs
    glfwSetWindowUserPointer(glfwWindow, inputs.get());
    glfwSetCursorPosCallback(glfwWindow, mousePosCallBack);
    glfwSetKeyCallback(glfwWindow, callbackKey);
    glfwSetMouseButtonCallback(glfwWindow, mouseButtonCallBack);
    glfwSetCharCallback(glfwWindow, charCallback);
    glfwSetFramebufferSizeCallback(glfwWindow, callback_size);

    // ativa blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // ativa o stencil
    glEnable(GL_STENCIL_TEST);

    tamanho = {800, 400};
}

void BubbleUI::novoContexto(GLFWwindow* window)
{
    if (!window) abort();   // Janela inválida
    contextos[window] = std::make_shared<Contexto>(window);
}

std::shared_ptr<BubbleUI::Contexto> BubbleUI::novoContexto(const char* nome_janela, const Vector4& pos_tam)
{
    size_t n_ctx = contextos.size();
    // Inicia a criação do contexto em uma nova thread
    std::thread([nome_janela, pos_tam]() mutable {
        inicializarContextoNaThread(nome_janela, pos_tam);
        }).detach();
    // Espera pela modificação no map
    std::unique_lock<std::mutex> lock(mtx);
    contexto_pronto.wait(lock, [&]() { return contextos.size() > n_ctx; });

    return prev(contextos.end())->second;
}

void BubbleUI::Contexto::adicionarVP(const bool preenchido)
{
    adicionarTarefa([this, preenchido]() {
        auto vp = new BubbleUI::Paineis::VisualizadorDeProjetos(preenchido);
        vp->definirContexto(contextos[glfwWindow]);
        paineis.emplace_back(vp);
        });
}

void BubbleUI::Contexto::atualizar()
{
    glfwPollEvents();

    // Executa as tarefas pendentes na fila
    {
        std::lock_guard<std::mutex> lock(mutexFilaTarefas);
        while (!filaDeTarefas.empty()) {
            auto tarefa = filaDeTarefas.front();
            filaDeTarefas.pop();
            tarefa();  // Executa a tarefa
        }
    }

    // Vetor de futures para armazenar as tarefas assíncronas de atualização dos paineis
    std::vector<std::future<void>> futures;

    // Inicia cada painel->atualizar() em uma thread separada
    for (auto& painel : paineis) {
        futures.push_back(std::async(std::launch::async, [&painel]() {
            painel->atualizar();
            }));
    }

    for (auto& future : futures) {
        future.wait();
    }
}
void BubbleUI::Contexto::renderizar() const
{
    glClearColor(0.1F, 0.1F, 0.1F, 1.F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, tamanho.width, tamanho.height);

    for (const auto& painel : paineis)
        painel->renderizar();

    glfwSwapBuffers(glfwWindow);
}

//static void BubbleUI::renderizarContexto(GLFWwindow* window)
//{
//    if (!window) abort();   // Janela inválida
//    if (contextos.find(window) == contextos.end()) abort(); // Não possui contexto para essa janela
//
//}

void BubbleUI::Contexto::adicionarTarefa(const std::function<void()>& tarefa) {
    std::lock_guard<std::mutex> lock(mutexFilaTarefas);
    filaDeTarefas.push(tarefa);
}

void BubbleUI::inicializarContextoNaThread(const char* nome_janela, const Vector4& pos_tam)
{
    // Inicializa o GLFW e verifica erros
    if (!glfwInit()) {
        std::cerr << "!";
        return;
    }

    // Cria a janela e contexto OpenGL
    auto window = glfwCreateWindow(pos_tam.w, pos_tam.h, nome_janela, NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "!!";
        return;
    }

    glfwSetWindowPos(window, pos_tam.x, pos_tam.y);
    glfwMakeContextCurrent(window);

    // Carrega o OpenGL usando GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        std::cerr << "!!!";
        return;
    }

    // Define o ícone da janela, se necessário
    auto icone_ = Bubble::Arquivadores::ImageLoader("icon.ico");
    const GLFWimage icone = icone_.converterParaGlfw();
    if (icone_.carregado) {
        glfwSetWindowIcon(window, 1, &icone);
    }

    // Define o contexto e armazena no map
     // Protege o acesso ao map
    {
        std::lock_guard<std::mutex> lock(mtx);
        contextos[window] = std::make_shared<BubbleUI::Contexto>(window);
    }

    contexto_pronto.notify_one();

    // Loop de renderização na mesma thread
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Atualização e renderização do contexto atual
        contextos[window]->atualizar();
        contextos[window]->renderizar();
    }

    // Limpa recursos ao encerrar
    contextos.erase(window);
    glfwDestroyWindow(window);
}