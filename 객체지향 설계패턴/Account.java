
public class Account {
	int balance;
	
	boolean withdrawReady = false;
	boolean depositReady = true;
	
	public Account(int balance) {
		this.balance = balance;
	}
    
    public synchronized void withdraw(int money){
        try{
        	while(!withdrawReady) wait();
        	if(Task.count >= 4) ;
        	else {
	        	withdrawReady = false;
	        	balance -= money;
	        	if(balance < 0) {
	        		balance += money;
	        		Thread thread = Thread.currentThread();
	        		Task.count += 1;
	        		System.out.println("Familly member "+thread.getName() + " trying to retrieving \t -$"+money);
	        		System.out.println("Not allowed, balance is not sufficient!");
	        		System.out.println("Total Saved: $"+ Task.totalDeposit+ " Total Spent: $" + Task.totalWithdraw);
	                System.out.println("\t Current balance:\t\t\t$" + balance);
	                Thread.sleep(1000);
		        	}
	        	
	        	else {
	        		Task.totalWithdraw += money;
	        		Thread thread = Thread.currentThread();
		            System.out.println("Familly member "+thread.getName() + " retrieving \t\t -$"+money);
		            System.out.println("\t New balance:\t\t\t\t$" + balance);
		            Thread.sleep(1000);
	        	}
	            depositReady = true;
	            notifyAll();
	            wait();
        	} 
        }catch (Exception e) {}
        
       
    }
    
    public synchronized void deposit(int money){

        try{
        	while(!depositReady) wait();
        	if(Task.count >= 4) ;
        	else {
				depositReady = false;
				balance += money;
				Task.totalDeposit += money;
				Thread thread = Thread.currentThread();
			    System.out.println("Familly member "+thread.getName() + " storing \t\t +$"+money);
			    System.out.println("\t New balance:\t\t\t\t$" + balance);
			    Thread.sleep(1000);
			    withdrawReady = true;
			    
			    notifyAll();
			    wait();
        	}
        }catch (Exception e) {}
    }
    
}

