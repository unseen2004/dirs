package main

import (
	"errors"
	"fmt"
	"math/rand/v2"
	"strconv"
	"sync"
	"time"
)

// allow only left, right, down, up
func move_col(out_x, out_y *int, dir pair) error {
	if dir.x == -1 && dir.y == 0 {
		*out_x = (*out_x - 1 + board_width) % board_width
	} else if dir.x == 1 && dir.y == 0 {
		*out_x = (*out_x + 1 + board_width) % board_width
	} else if dir.x == 0 && dir.y == -1 {
		*out_y = (*out_y - 1 + board_width) % board_height
	} else if dir.x == 0 && dir.y == 1 {
		*out_y = (*out_y + 1 + board_width) % board_height
	} else {
		return errors.New("Wrong direction params")
	}

	return nil
}

func run_traveler_col(id int, symbol rune) (path []string, err error) {
	directions := [4]pair{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}
	pos := pair{rand.IntN(board_width), rand.IntN(board_height)}
	n := rand.IntN(max_steps-min_steps) + min_steps
	path = make([]string, n)
	start := time.Now()
	for i := 0; i < n; i++ {
		wait := min_delay + rand.Float64()*(max_delay-min_delay)
		time.Sleep(time.Duration(wait * float64(time.Second)))
		err := move(&pos.x, &pos.y, directions[rand.IntN(4)])
		if err != nil {
			return nil, err
		}
		path[i] = "time:" + fmt.Sprintf("%.2f", time.Since(start).Seconds()) + " id:" + strconv.Itoa(id) + " x:" + strconv.Itoa(pos.x) + " y:" + strconv.Itoa(pos.y) + " symbol:" + string(symbol)
	}

	return path, nil
}

func run_col() {

	fmt.Printf("Number of travelers:%d W:%d H:%d \n", nr_of_travelers, board_width, board_height)

	var wg sync.WaitGroup
	wg.Add(nr_of_travelers)

	for i := 0; i < nr_of_travelers; i++ {
		go func() {
			var path, err = run_traveler(i, rune(i+65))
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
