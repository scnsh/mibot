package main

import (
	"log"
	"os"
	"bufio"
    "fmt"

    mqtt "github.com/eclipse/paho.mqtt.golang"
)

func main() {
    opts := mqtt.NewClientOptions()
    opts.AddBroker("tcp://localhost:1883")
    c := mqtt.NewClient(opts)

    if token := c.Connect(); token.Wait() && token.Error() != nil {
        log.Fatalf("Mqtt error: %s", token.Error())
    }

    reader := bufio.NewReader(os.Stdin)
    fmt.Println("send commands (w:forward d:left a:right s:backward enter:send ctrl-c:exit)")

    // tc := time.Tick(200 * time.Millisecond)
    for {
        b, _ := reader.ReadByte()
        msg := string(b)
        if b != 0x0a {
            fmt.Println("send:", msg, len(msg))
            token := c.Publish("m5stack/control", 0, false, msg)
            token.Wait()
        }
        // select {
        // case <-tc:
        // }
    }
}