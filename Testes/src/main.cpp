#include <gtest/gtest.h>
#include "src/nucleo/scenemanager.h"

int main(int argc, char** argv)
{

    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
    return 0;
}
TEST(SceneManagerTest, Testando_Adicionar_Cena)
{
    Bubble::Nucleo::SceneManager sm;

    auto cena = std::make_shared<Bubble::Nucleo::Scene>("CENA GTEST");

    sm.adicionarCena(cena);

    EXPECT_EQ(sm.numeroDeCenas(), 1);
}
TEST(SceneManagerTest, Testando_Execao_Shader)
{
    Bubble::Nucleo::SceneManager sm;

    auto cena = std::make_shared<Bubble::Nucleo::Scene>("CENA GTEST");

    sm.adicionarCena(cena);

    EXPECT_THROW(sm.carregarCena(0), ShaderException);
}