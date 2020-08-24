// A simple demonstration of wait() and notify().

class SyncOb {

  boolean ready = false;


  synchronized void waitFor() {
    String thrdName = Thread.currentThread().getName();
    System.out.println(thrdName + " is entering waitFor().");

    System.out.println(thrdName +
                       " calling wait() to wait for" +
                       " notification to proceed.\n");

    try {


      while(!ready) wait();

    } catch(InterruptedException exc) {
      System.out.println("Interrupted.");
    }

    System.out.println(thrdName +
                       " received notification and is" +
                       " resuming execution.");
  }


  synchronized void goAhead() {
    String thrdName = Thread.currentThread().getName();
    System.out.println("\n" + thrdName +
                       " thread calling notify() inside goAhead().\n" +
                       "This will let MyThread resume execution.\n");


    ready = true;
    notify();
  }
}


class MyThread implements Runnable {
  SyncOb syncOb;

  // Construct a new thread.
  MyThread(String name, SyncOb so) {
    syncOb = so;
    new Thread(this, name).start();
  }

  // Begin execution of the thread.
  public void run() {
    syncOb.waitFor();
  }
}

class ThreadComDemo {
  public static void main(String args[]) {

    try {
      SyncOb sObj = new SyncOb();


      new MyThread("MyThread", sObj);


      for(int i=0; i < 10; i++) {
        Thread.sleep(250);
        System.out.print(".");
      }
      System.out.println();


      sObj.goAhead();



    } catch(InterruptedException exc) {
      System.out.println("Main thread interrupted.");
    }
  }
}