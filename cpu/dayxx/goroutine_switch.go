package main

import "fmt"
import "time"
import "sync"

func tyr(channels [2](chan int), idx int, loops int, wg *sync.WaitGroup) {
	for i := 0; i < loops; i++ {
		channels[idx] <- 1
		<-channels[1-idx]
	}
	wg.Done()
}

func main() {
	var channels = [2]chan int{make(chan int, 1), make(chan int, 1)}
	nloops := 10000000
	start := time.Now()
	var wg sync.WaitGroup
	wg.Add(2)
	go tyr(channels, 0, nloops, &wg)
	go tyr(channels, 1, nloops, &wg)
	wg.Wait()
	elapsed := time.Since(start).Seconds()
	fmt.Printf("%fs elapsed\n", elapsed)
	fmt.Printf("%f us per switch\n", 1e6*elapsed/float64(2*nloops))
}