package main

import (
	"fmt"
	"sync"
)

func main() {
	fmt.Printf("Number of travelers:%d W:%d H:%d \n", nr_of_travelers, board_width, board_height)

	var wg sync.WaitGroup
	wg.Add(nr_of_travelers)

	for i := 0; i < nr_of_travelers; i++ {
		go func() {
			var path, err = run_traveler(i, rune(i+96))
			if err == nil {
				printer(path)
			} else {
				fmt.Println(err)
			}
			wg.Done()
		}()
	}

	wg.Wait()
}
