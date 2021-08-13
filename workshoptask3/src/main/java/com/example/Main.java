package com.example;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import java.io.IOException;
import akka.event.Logging;
import akka.event.LoggingAdapter;

class Main {

    public static void main(String[] args) {
        //Starting the actor system
        ActorSystem system = ActorSystem.create();
        final LoggingAdapter log = Logging.getLogger(system,system);
        log.info("Starting the Producer");

        //Creating Producer actor
        ActorRef ProducerRef = system.actorOf(Props.create(Producer.class));
        ProducerRef.tell(new Integer(0),ProducerRef);

        //terminate the actors
        try {
            System.out.println("Press ENTER to end program.");
            System.in.read();
        }
        catch (IOException ignored) { }
        finally {
            system.terminate();
            log.info("Akka System Terminated.");
        }
    }
}
