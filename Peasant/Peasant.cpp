// Peasant.cpp : Defines the entry point for the console application.
//

#include "Peasant.h"


/*
	DONE - Preciso adicionar o controle ao PeasantObject agora, no sentido de que quando ele é carregado, alguma função
	deve ser chamada para criar por exemplo os buffers e outros detalhes importantes...
	- Devemos talvez ter multiplas funções para isso (uma que processa os dados carregados, outra que cria os buffers
	e coloca eles no render graph, etc etc).
	DONE - Preciso adicionar as estruturas de data e data size para o PeasantObject (e suas respectivas funções de atualização)
	DONE - Preciso organizar essa classe também (PeasantObject) ou usar uma outra de base para o load acontecer.
	- Fazer com que o PeasantObjectLoader coloque as variáveis de controle (caso não precisamos dar "build" no objeto)
	DONE - Mudar a chamada the ReleaseObject() (factory) do Deleter para o Manager, de forma que a deleção dele seja sincrona
	(adicionando em um queue e verificando se ainda está carregado e se a data é válida).
	- Ver como podemos fazer para certas chamadas do manager permitirem o uso de jobs (o construct pode ser feito usando 
	várias threads, não?).
*/

int main()
{
    return 0;
}