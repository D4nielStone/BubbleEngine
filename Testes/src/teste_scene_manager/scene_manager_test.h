#include <gtest/gtest.h>
#include <Bubble-Engine1/src/nucleo/scenemanager.h>

using namespace Bubble::Nucleo;

TEST(SceneManagerTest, TesteAdicionarCena)
{
    
    SceneManager sm;

    auto cena = std::make_shared<Scene>("CENA GTEST");
    
    sm.adicionarCena(cena);

    EXPECT_EQ(sm.numeroDeCenas(), 1); 
}
TEST(SceneManagerTest, TesteCarregarCena)
{
    SceneManager sm;

    auto cena = std::make_shared<Scene>("CENA GTEST");

    sm.adicionarCena(cena);

    sm.carregarCena(0);

    EXPECT_EQ(sm.cenaAtual(), 0); 
}