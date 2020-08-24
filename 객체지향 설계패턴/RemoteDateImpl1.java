import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.Date;

public class RemoteDateImpl1 extends UnicastRemoteObject implements RemoteDate
{
	
	
    public RemoteDateImpl1() throws RemoteException {   }

    public  Date getDate() throws RemoteException {
        return new Date();
    }

   public static void main(String[] args)  {
        try {

            RemoteDate dateServer1 = new RemoteDateImpl();

            Naming.rebind("DateServer1", dateServer1);
            
            

            System.out.println("DateServer bound in registry");
            
            String host = "rmi://127.0.0.1/DateServer2";
            String a[] = Naming.list("rmi://127.0.0.1");
            System.out.println("bounding checking...");
            int i = 0;
            while(a.length < 2) { 
          	  if (i == 0) System.out.println("please wait until another server bounding in resistry...");
          	  a = Naming.list("rmi://127.0.0.1");
          	  i++;
            }
            RemoteDate dateServer2 = (RemoteDate)Naming.lookup(host);
            System.out.println(dateServer2.getDate());
        }
        catch (Exception e) {
            System.err.println(e);
        }
    }





}
