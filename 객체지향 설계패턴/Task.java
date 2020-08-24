
public class Task implements Runnable {
	Account acc;
	private String name;
	public static int count = 0;
	public static int totalWithdraw = 0;
	public static int totalDeposit = 0;
	
	public Task(Account acc) {
		this.acc = acc;
	}
	
	public void setName(String num) {
		name = num;
	}
	
	public void run() {
        while(this.count < 4){
            // 100, 200, 300 중의 한 값을 임의로 선택해서 출금(withDraw)한다.
            int wmoney = (int)(Math.random() * 40 + 40);
            int dmoney = (int)(Math.random() * 40 + 1);
            if(acc.depositReady && this.count < 4) acc.deposit(dmoney);
            else if(acc.withdrawReady && this.count < 4) acc.withdraw(wmoney); 
        }
    }
}
