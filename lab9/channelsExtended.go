//introduz canais na aplicação Go
package main

import (
	"fmt"
)

func tarefa(str chan string) {
	
    var msg = "" //cria string buffer para a Goroutine

    msg = <-str //escreve no canal

    fmt.Println("Main diz:", msg)

	str <- "Oi main, bom dia, tudo bem?"

    msg = <-str

    fmt.Println("Main diz:", msg)

    str <- "Certo, entendido."

    msg = <-str

	fmt.Println("Main diz:", msg)

    str <- "finalizando"
}

func main() {
	var msg = "" //string vazia
	
	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string)
	
	//cria um goroutine que executará a função 'tarefa'
	go tarefa(str)
    
    str <- "Olá, Goroutine, bom dia!"

	//lê do canal
	msg = <-str

	fmt.Println("Goroutine diz:", msg)

    str <- "Tudo bem. Vou terminar tá?"

    msg = <-str

	fmt.Println("Goroutine diz:", msg)

    str <- "finalizando"

    msg = <-str

    fmt.Println("Goroutine diz:", msg)

}
