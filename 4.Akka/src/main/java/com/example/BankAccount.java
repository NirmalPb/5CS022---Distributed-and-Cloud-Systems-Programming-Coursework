package com.example;
import akka.actor.AbstractActor;
import akka.event.Logging;
import akka.event.LoggingAdapter;

public class BankAccount extends AbstractActor {
    private final LoggingAdapter log = Logging.getLogger(getContext().getSystem(), this);
    private double balance;
    private int transactions;

    // initialise the balance
    public BankAccount (){
        balance = 100.00;
        transactions =0;
        log.info("BankAccount initial balance value : '{}'", this.balance);
    }

    @Override
    public Receive createReceive() {
        return receiveBuilder()
                .match(Witdrawal.class, this::onWithdrawal)
                .match(Deposit.class, this::onDeposit)
                .build();
    }

    //on Withdrawal
    private void onWithdrawal(Witdrawal msg) {
        log.info("BankAccount received Withdrawal message with value of : '{}'", msg.text);
        this.balance = balance + Double.parseDouble(msg.text);
        log.info("BankAccount balance value : '{}'", this.balance);

        //terminating if all 10 transactions are completed
        transactions ++;
        if (transactions==10){
            log.info("Program Terminated.");
        }
    }

    //on Deposit
    private void onDeposit(Deposit msg) {
        log.info("BankAccount received Deposit message with value of : '{}'", msg.text);
        this.balance += Double.parseDouble(msg.text);
        log.info("BankAccount balance value : '{}'", this.balance);

        //terminating if all 10 transactions are completed
        transactions ++;
        if (transactions==10){
            log.info("Program Terminated.");
        }
    }

}