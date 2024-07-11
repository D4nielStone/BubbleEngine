#include <gtest/gtest.h>
#include "src/nucleo/scenemanager.h"

TEST(SceneManagerTest, TesteAdicionarCena)
{
    Bubble::Nucleo::SceneManager sm;

    auto cena = std::make_shared<Bubble::Nucleo::Scene>("CENA GTEST");
    
    sm.adicionarCena(cena);

    EXPECT_EQ(sm.numeroDeCenas(), 1); 
}
TEST(SceneManagerTest, TesteCarregarCena)
{
    Bubble::Nucleo::SceneManager sm;

    auto cena = std::make_shared<Bubble::Nucleo::Scene>("CENA GTEST");

    sm.adicionarCena(cena);

    sm.carregarCena(0);

    EXPECT_EQ(sm.cenaAtual(), 0); 
}