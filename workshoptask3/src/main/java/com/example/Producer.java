package com.example;
import akka.actor.AbstractActor;
import akka.actor.ActorRef;
import akka.actor.Props;
import akka.event.Logging;
import akka.event.LoggingAdapter;

import java.util.Random;

public class Producer extends AbstractActor {
    private final LoggingAdapter log = Logging.getLogger(getContext().getSystem(), this);

    public static Props props() {
        return Props.create(Producer.class, Producer::new);
    }

    @Override
    public Receive createReceive() {
        return receiveBuilder()
                .match(Integer.class, this::onInteger)
                .match(String.class, this::onString)
                .build();
    }

    /**
     * Start the supervisor
     * @param msg Received message
     * */
    private void onInteger(Integer msg) {
        ActorRef SupervisorRef = getContext().getSystem().actorOf(Props.create(Supervisor.class));
        Random r = new Random();
        int low = 10000;
        int high = 100000;
        SupervisorRef.tell(new String("Start Workers"),getSelf());
        //Sending Numbers
        for (int i=0;i<1000;i++){
            SupervisorRef.tell(new Integer(r.nextInt(high-low) + low),getSelf());
        }
    }

    /**
     * Display Results
     * @param msg Received message
     * */
    private void onString(String msg) {
        log.info(msg);
    }

}