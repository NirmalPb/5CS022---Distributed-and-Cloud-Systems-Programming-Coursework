package com.example;
import akka.actor.AbstractActor;

public class Worker extends AbstractActor {
    @Override
    public Receive createReceive() {
        return receiveBuilder()
                .match(Integer.class, this::onInteger)
                .build();
    }

    /**
     * If a number is prime send back to the producer
     * @param msg Number
     * */
    private void onInteger(Integer msg) {
        boolean flag = false;
        for (int i = 2; i <= msg / 2; ++i) {
            // condition for nonprime number
            if (msg % i == 0) {
                flag = true;
                break;
            }
        }
        //send prime numbers back to producer
        if (!flag){
            getSender().tell(new String(msg + " Is a Prime Number"),getSelf());
        }
    }

}
