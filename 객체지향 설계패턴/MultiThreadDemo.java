import java.util.Scanner;


public class MultiThreadDemo {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		
		System.out.print("Initial Balance : ");
		int balance = sc.nextInt();
		System.out.print("The number of family members : ");
		int numberOfMember = sc.nextInt();
		
		Account acc = new Account(balance);
		
		Task task = new Task(acc);
		
		Thread[] mt = new Thread[numberOfMember+1];
		
		for(int i=1;i<=numberOfMember;i++) {
			mt[i] = new Thread(task);
			mt[i].setName(Integer.toString(i));
		}
		for(int j=1;j<=numberOfMember;j++) {
			mt[j].start();
		}
	}	
}
