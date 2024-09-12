Respostas lab 5 prog conc

1)1- A ordem de impressão requerida pela aplicação é que a thread A imprima "HELLO" pelo menos duas vezes antes de a thread B imprimir "BYEBYE".

2)1- Sim os requisitos foram atendidos. As variáveis sinalizadoras "chegada_cond" e "sentado_cond" performam da maneira esperada e bloqueiam a execução das threads 3, 4 e 5 até o devido momento de executarem.

3)2- Não. As threads estão executando sem sincronização, apenas resolvendo seus passos de forma desorganizada até que todas tenham completado todos os seus passos, independentemente da ordem.
3)3- Dessa vez as threads executam um passo e ao invés de seguir direto para o próximo elas esperam até que todas as outras threads tenham completado o mesmo passo, independente da ordem em que as threads executam elas sempre param na barreira e esperam até que as outras cheguem.
