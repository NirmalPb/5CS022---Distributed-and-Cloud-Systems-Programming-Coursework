package com.example;

import akka.actor.AbstractActor;
import akka.actor.ActorRef;
import akka.actor.Props;

import java.util.ArrayList;

public class Supervisor extends AbstractActor {
    public static Props props() {
        return Props.create(Supervisor.class, Supervisor::new);
    }
    int workerID;
    Supervisor(){
        workerID=0;
    }

    @Override
    public Receive createReceive() {
        return receiveBuilder()
                .match(String.class, this::onString)
                .match(Integer.class, this::onInteger)
                .build();
    }

    //Creating 10 Workers on Startup
    ArrayList<ActorRef> workersRef= new ArrayList<ActorRef>();
    /**
     * Create Worker Actors
     * @param msg received Massage
     * */
    private void onString(String msg) {
        for (int i = 0; i < 10; i++){
            workersRef.add(getContext().getSystem().actorOf(Props.create(Worker.class)));
        }
    }

    /**
     * Sending numbers to workers
     * @param msg Numbers
     * */
    private void onInteger(Integer msg) {
        workersRef.get(workerID).forward(msg,getContext());
        workerID++;
        if (workerID==9){
            workerID=0;
        }
    }
}
