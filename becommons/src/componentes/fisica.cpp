#include "fisica.hpp"
#include "transformacao.hpp"
#include <src/nucleo/fase.hpp>
#include "renderizador.hpp"

// Construtor para forma genérica
bubble::fisica::fisica(btCollisionShape* forma, btScalar massa, btVector3 posicaoInicial)
    : forma(forma), malha(false), massa(massa), posicaoInicial(posicaoInicial)
{
    init();
}
void bubble::fisica::init()
{
    btVector3 inertia(0, 0, 0);
    if (massa > 0) {
        forma->calculateLocalInertia(massa, inertia);
    }

    estadoDeMovimento = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), posicaoInicial));
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(massa, estadoDeMovimento, forma, inertia);
    corpoRigido = new btRigidBody(rigidBodyCI);
    corpoRigido->setRollingFriction(0.1);
}
// Construtor para criação de malha
bubble::fisica::fisica(bool malha, btScalar massa, btVector3 posicaoInicial)
    : malha(malha), massa(0), posicaoInicial(posicaoInicial)
{
}

// Destrutor
bubble::fisica::~fisica()
{
    fase_atual->sfisica.remover(corpoRigido);
    delete corpoRigido;
    delete estadoDeMovimento;
    delete forma;
}

// Obter o corpo rígido
btRigidBody* bubble::fisica::obterCorpoRigido()
{

    if (malha &&!corpoRigido) {
        criarMalha();
        init();
        malha = false; // Garante que as malhas sejam criadas apenas uma vez
    }
    return corpoRigido;
}

// Criar forma para múltiplas malhas
void bubble::fisica::criarMalha()
{
    // Obtém as malhas do modelo associado ao objeto
    auto modelo = fase_atual->obterRegistro()->obter<bubble::renderizador>(meu_objeto)->modelo;
    auto& malhas = modelo->malhas;

    // Criar o btTriangleIndexVertexArray para todas as malhas
    btTriangleIndexVertexArray* indexVertexArray = new btTriangleIndexVertexArray();

    for (const auto& malha : malhas) {
        // Supondo que malha tenha 'vertices' e 'indices'
        const auto& vertices = malha.vertices;
        const auto& indices = malha.indices;

        btIndexedMesh mesh;
        mesh.m_numTriangles = indices.size() / 3;
        mesh.m_triangleIndexBase = (const unsigned char*)indices.data();
        mesh.m_triangleIndexStride = 3 * sizeof(unsigned int);
        mesh.m_numVertices = vertices.size();
        mesh.m_vertexBase = (const unsigned char*)vertices.data();
        mesh.m_vertexStride = sizeof(vertice); // Supondo que Vertex tem posições contíguas
        mesh.m_indexType = PHY_INTEGER;
        mesh.m_vertexType = PHY_FLOAT;

        indexVertexArray->addIndexedMesh(mesh, PHY_INTEGER);
    }

    // Cria a forma de colisão como um btBvhTriangleMeshShape
    forma = new btBvhTriangleMeshShape(indexVertexArray, true);
}

// Atualizar transformação
void bubble::fisica::atualizarTransformacao()
{

    btTransform bt;
    estadoDeMovimento->getWorldTransform(bt);

    _Mtransformacao = fase_atual->obterRegistro()->obter<bubble::transformacao>(meu_objeto).get();
    _Mtransformacao->posicao = { bt.getOrigin().getX(), bt.getOrigin().getY(), bt.getOrigin().getZ() };
    _Mtransformacao->rotacao = { bt.getRotation().getX(), bt.getRotation().getY(), bt.getRotation().getZ() };
}

// Aplicar força
void bubble::fisica::aplicarForca(const glm::vec3& vetor)
{
    corpoRigido->applyCentralForce({ vetor.x, vetor.y, vetor.z });
}

// Aplicar velocidade
void bubble::fisica::aplicarVelocidade(const glm::vec3& velocidade)
{
    corpoRigido->setLinearVelocity(btVector3(velocidade.x, velocidade.y, velocidade.z));
}

// Obter velocidade
glm::vec3 bubble::fisica::obterVelocidade() const
{
    return { corpoRigido->getLinearVelocity().getX(),
            corpoRigido->getLinearVelocity().getY(),
            corpoRigido->getLinearVelocity().getZ() };
}