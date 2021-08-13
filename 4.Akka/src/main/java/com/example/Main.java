package com.example;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import java.io.IOException;
import java.util.Random;

import akka.event.Logging;
import akka.event.LoggingAdapter;

class Main {

    public static void main(String[] args) {
        //create actor sys: with bank account actor
        ActorSystem system = ActorSystem.create();
        final LoggingAdapter log = Logging.getLogger(system,system);
        log.info("Creating Bank Account");
        ActorRef bankARef = system.actorOf(Props.create(BankAccount.class));

        //creating random numbers between -100 and 100 sending to bank account with appropriate message
        Random r = new Random();
        final int low = -100;
        final int high = 100;

        for (int i =0; i<10; i++){
            double amount = r.nextInt(high-low) + low;
            if (amount > 0) {
                bankARef.tell(new Deposit("" + amount), bankARef);
            }else if (amount <0){
                bankARef.tell(new Witdrawal("" + amount), bankARef);
            }
        }

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
