package x509;

import java.io.*;
import java.util.*;
import java.security.cert.*;
import java.security.cert.Certificate;

public class test {
	public static void main(String[] args) throws CertificateException, FileNotFoundException {
		
		CertificateFactory cFactory = CertificateFactory.getInstance("X.509");
		//读入cer文件
		FileInputStream inputStream = new FileInputStream("G:\\githubCode\\java code\\x509\\src\\x509\\x509.cer");	
		Certificate certificate = cFactory.generateCertificate(inputStream);
		//创建x509证书实例
		X509Certificate x509Certificate = (X509Certificate)certificate;
		
		//输出：
		System.out.println("Version ："+x509Certificate.getVersion());
		System.out.println("Serial Number ："+x509Certificate.getSerialNumber());
		System.out.println("Signature Algorithm ："+x509Certificate.getSigAlgName());
		System.out.println("Issuer ："+ x509Certificate.getIssuerX500Principal());
		//获取日期
		Date notBefore = x509Certificate.getNotBefore();
		Date notAfter  = x509Certificate.getNotAfter();
		System.out.println("Validate: ");
		System.out.println("Not before: " + notBefore);
		System.out.println("Not after: " + notAfter);
		
		String aString = x509Certificate.getSubjectX500Principal().toString();
		System.out.println("Subject: " + aString);//SUBJECT
		
		//获取公钥编码：
		byte[] pkBytes = x509Certificate.getPublicKey().getEncoded();  
		String pkey = x509Certificate.getPublicKey().toString();
		System.out.println("Public Key:");        
		System.out.print(pkey.substring(0, 30));
		System.out.print(pkey.substring(30,41));
		for(int i = 42;i < pkBytes.length - 25;i++)
			System.out.print(pkBytes[i] + ",");
		System.out.println(pkey.substring(pkey.length() - 25,pkey.length()));
		
		System.out.println("Subject name: " + x509Certificate.getSubjectDN());
		System.out.println("Issuer: "+ x509Certificate.getIssuerX500Principal());//issuer
		System.out.println("Issuer Unique Identifier: " + x509Certificate.getIssuerUniqueID());//x509Certificate.getCriticalExtensionOIDs());
		System.out.println("Subject Unique Identifier: "+ x509Certificate.getSubjectUniqueID());	
	}
	
	
}
