
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.Date;

public class RemoteDateImpl2 extends UnicastRemoteObject implements RemoteDate
{
	
   public RemoteDateImpl2() throws RemoteException {  }
   
   public  Date getDate() throws RemoteException {
       return new Date();
   }
	
   public static void main(String args[]) {
    try {
        /**
         * We only need this for loading remote classes
         * System.setSecurityManager(new RMISecurityManager());
         */
    	
    	RemoteDate dateServer2 = new RMIClient();

        Naming.rebind("DateServer2", dateServer2);

        System.out.println("DateServer2 bound in registry");
        
      String host = "rmi://127.0.0.1/DateServer1";
      String a[] = Naming.list("rmi://127.0.0.1");
      System.out.println("bounding checking...");
      int i = 0;
      while(a.length < 2) { 
    	  if (i == 0) System.out.println("please wait until another server bounding in resistry...");
    	  a = Naming.list("rmi://127.0.0.1");
    	  i++;
      }
      RemoteDate dateServer= (RemoteDate)Naming.lookup(host);
      System.out.println(dateServer.getDate());
    }
    catch (Exception e) {
        System.err.println(e);
    }
   }
}
