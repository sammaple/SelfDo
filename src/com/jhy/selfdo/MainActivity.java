package com.jhy.selfdo;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener {

	protected static final int THREAD = 0;
	Button bt;
	Button bt_datachmod;
	TextView tx;
	
	String[] vm_property ={
			"java.vm.name",
			"java.vm.specification.vendor",
			"java.vm.vendor",
			"java.vm.specification.name",
			
			"java.specification.name",
			"java.specification.vendor",
			
			"java.vendor",
			"ro.yunos.version",
			
			"lemur.vm.version"
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		bt = (Button) findViewById(R.id.button_start);
		tx = (TextView) findViewById(R.id.textView_judge);
		bt_datachmod = (Button) findViewById(R.id.button_chmod);
		
		bt.setOnClickListener(this);
		bt_datachmod.setOnClickListener(this);
	}

	@Override
	public void onClick(View arg0) {
		if (arg0.getId() == R.id.button_start) {
			startJudge();
		}else if(arg0.getId() == R.id.button_chmod){

			try {
				execSuCommand("busybox chmod 777 -R /data");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	}

	Handler mhadler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case THREAD:
				tx.setText((String) msg.obj);
				break;
			}
			super.handleMessage(msg);
		}
	};
	
	class cleanjob implements Runnable{

		@Override
		public void run() {
			try {
				//execCommand("getprop");
				//execCommand("");
				/*proc = runtime.exec("chmod 777 -R /mnt/sdcard/DCIM/.thumbnails");
				proc.waitFor();*/
				//proc = runtime.exec("su -c mkdir /mnt/sdcard/DCIM/ddd");
				//proc = runtime.exec("su -c rm -rf /mnt/sdcard/DCIM/dd");

				execCommandArray(new String[]{"sh","-c","getprop|grep ip"});
				
				execSuCommand("rm -rf /mnt/sdcard/DCIM/.thumbnails");//ɾ������ͼ����
				//execCommand("ls");
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			/*Properties p = System.getProperties();
			
			StringBuilder strb = new StringBuilder(tx.getText());
			for (String str : vm_property) {
				System.err.println(p.get(str));
				
				strb.append(str+":"+p.get(str)+"\n");
			}
			
			tx.setText(strb.toString());*/
		}
		
	}

	private void startJudge() {
		//Message m = mhadler.obtainMessage(THREAD, "test");
		//m.sendToTarget();
	
		new Thread(new cleanjob()).start();
	
	}
	
	
	public static String execSuCommand(String cmd) throws IOException
    {

		System.err.println("suִ�п�ʼ");
        Process process = Runtime.getRuntime().exec("su");  
        DataOutputStream os = new DataOutputStream(process.getOutputStream());  
        os.writeBytes(cmd+"\n");
        os.flush();
        os.writeBytes("exit\n");
        os.flush();
        
        BufferedReader reader = new BufferedReader(new InputStreamReader(  
                process.getInputStream()));  
        int read;  
        char[] buffer = new char[4096];  
        StringBuffer output = new StringBuffer();  
        while ((read = reader.read(buffer)) > 0) {  
            output.append(buffer, 0, read);  
        }  
        reader.close();
        os.close();
        

		System.err.println("suִ�н���");
        return output.toString();
    }

	public void execCommand(String command) throws IOException {
		Runtime runtime = Runtime.getRuntime();
		Process proc = runtime.exec(command);

		System.err.println("ִ�п�ʼ");

		try {
			System.err.println("ִ�п�ʼ2");

			if (proc.waitFor() != 0) {

				System.err.println("exit value1 = " + proc.exitValue());
				BufferedReader reader = new BufferedReader(
						new InputStreamReader(proc.getErrorStream()));
				int read;
				char[] buffer = new char[4096];
				StringBuffer output = new StringBuffer();
				while ((read = reader.read(buffer)) > 0) {
					output.append(buffer, 0, read);
				}
				reader.close();
				
				System.out.println(output.toString());


			} else{

				System.err.println("ִ�����");
			}

				BufferedReader reader = new BufferedReader(
						new InputStreamReader(proc.getInputStream()));
				int read;
				char[] buffer = new char[4096];
				StringBuffer output = new StringBuffer();
				while ((read = reader.read(buffer)) > 0) {
					output.append(buffer, 0, read);
				}
				reader.close();

				System.out.println(output.toString());
			

		} catch (InterruptedException e) {

			System.err.println(e);

		}

	}
	
	
	
	public void execCommandArray(String[] command) throws IOException {
		Runtime runtime = Runtime.getRuntime();

		//Process proc = runtime.exec(new String[]{"sh","-c","getprop|grep ip"});
		Process proc = runtime.exec(command);

		System.err.println("execCommandArray��ʼ");
		try {

			if (proc.waitFor() != 0) {

				System.err.println("exit value = " + proc.exitValue());
				
				if(proc.exitValue() == 1){

					System.err.println("execCommandArrayִ�з����޽�����߲�ѯΪ��");
				}
				

				BufferedReader reader = new BufferedReader(
						new InputStreamReader(proc.getErrorStream()));
				int read;
				char[] buffer = new char[4096];
				StringBuffer output = new StringBuffer();
				while ((read = reader.read(buffer)) > 0) {
					output.append(buffer, 0, read);
				}
				reader.close();
				
				System.out.println(output.toString());

			} else {
				System.err.println("execCommandArray����");
			}

				
				BufferedReader reader = new BufferedReader(
						new InputStreamReader(proc.getInputStream()));
				int read;
				char[] buffer = new char[4096];
				StringBuffer output = new StringBuffer();
				while ((read = reader.read(buffer)) > 0) {
					output.append(buffer, 0, read);
				}
				reader.close();

				System.out.println(output.toString());
			

		} catch (InterruptedException e) {

			System.err.println(e);

		}

	}


}
