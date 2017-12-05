/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;





/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	   int numTestsFailed = 0;
	   int numTestsTotal = 0;
	   int testResult;
	   
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println("\nValidator Setting: Allowing all schemes: \n");
	    
	   //Create URL/expected result pairs to manually test
	   //TODO:Add more tests here
	   ResultPair[] manualTestURLs = {
		   new ResultPair("http://www.example.com", true),
		   new ResultPair("www.example.com", true)
	   	};
		numTestsTotal = manualTestURLs.length;
		   
	   //Run tests and tally the number of failures
	   System.out.println("\n=====================================================================================================");	 
	   System.out.println("Running Manual Tests");
	   System.out.println("_____________________________________________________________________________________________________");
	   
	   String testInfo = "";
	   for(int i = 0; i < numTestsTotal; i++){
		   
		   testResult = runTest(manualTestURLs[i], testInfo, true, UrlValidator.ALLOW_ALL_SCHEMES);
		   if(testResult == 1)
		   {
			   numTestsFailed++;
		   }
	   }
	   
	   System.out.println("_____________________________________________________________________________________________________");
	   System.out.println("Manual Test Results:  " + numTestsTotal + " tests run, " + numTestsFailed + " tests failed.");
	   System.out.println("=====================================================================================================\n");	
	   
   }
   
   // Partition testing: Scheme
   public void testSchemePartition()
   {
	   int numTestsFailed = 0;
	   int numTestsTotal = 0;
	   int testResult;
	   
	   //Create URL/expected result pairs to test scheme partition 
	   ResultPair[] partitionTestSchemes = {
		   new ResultPair("http://", true),
		   new ResultPair("https://", true),
		   new ResultPair("ftp://", true),
		   new ResultPair("", true),
		   new ResultPair("h3tp://", true),
		   new ResultPair("h12-+.-://", true),
		   new ResultPair("http", false),
		   new ResultPair("://", false),
		   new ResultPair("http//", false),
		   new ResultPair("http:", false),
		   new ResultPair("http:://", false),
		   new ResultPair("http:/", false),
		   new ResultPair("http:///", false),
		   new ResultPair("3tp://", false),
	   };
	   numTestsTotal = partitionTestSchemes.length;
	   
	   //Run tests and tally the number of failures
	   System.out.println("\n=====================================================================================================");	 
	   System.out.println("Running Partition Test for Scheme");
	   System.out.println("_____________________________________________________________________________________________________");
	   
	   for(int i = 0; i < numTestsTotal; i++){
		   //Create new URL with selected scheme, and validate.
		   ResultPair testToRun = new ResultPair(partitionTestSchemes[i].item + "www.example.com", partitionTestSchemes[i].valid);
		   String testInfo = "Scheme:'" + partitionTestSchemes[i].item + "'";
		   testResult = runTest(testToRun, testInfo, true, UrlValidator.ALLOW_ALL_SCHEMES);
		   if(testResult == 1)
		   {
			   numTestsFailed++;
		   }
	   }
	   System.out.println("_____________________________________________________________________________________________________");
	   System.out.println("Scheme Partition Test Results:  " + numTestsTotal + " tests run, " + numTestsFailed + " tests failed.");
	   System.out.println("=====================================================================================================\n");	
   }
   
// Partition testing: Authority
   public void testAuthorityPartition(){
	   int numTestsFailed = 0;
	   int numTestsTotal = 0;
	   int testResult;
	   
	   //Create URL/expected result pairs to test authority partition 
	   ResultPair[] partitionTestAuthority = {
		   new ResultPair("www.example.com", true),
		   new ResultPair("example.com", true),
		   new ResultPair("userid:password@example.com", true),
		   new ResultPair("255.255.255.255/", true),
		   new ResultPair("1337.net/", true),
		   new ResultPair("example.it", true),
		   new ResultPair("example.c0m", false),
		   new ResultPair("example.rt", true),
		   new ResultPair("example.-cm", false),
		   new ResultPair("256.256.256.256", false),
		   new ResultPair("", false),
		   new ResultPair(".com", false),
		   new ResultPair("@.com", false),
		   new ResultPair(".", false),
		   new ResultPair("125.125.125", false),
		   new ResultPair("0.0.0.0.0", false),
		   new ResultPair("255.255.255", false),
		   new ResultPair("255", false),
		   new ResultPair("example.c m", false)
	   };
	   numTestsTotal = partitionTestAuthority.length;
	   
	   //Run tests and tally the number of failures
	   System.out.println("\n=====================================================================================================");	 
	   System.out.println("Running Partition Test for Authority");
	   System.out.println("_____________________________________________________________________________________________________");
	   
	   for(int i = 0; i < numTestsTotal; i++){
		   //Create new URL with selected authority, and validate
		   ResultPair testToRun = new ResultPair("http://" + partitionTestAuthority[i].item, partitionTestAuthority[i].valid);
		   String testInfo = "Authority:'" + partitionTestAuthority[i].item + "'";
		   testResult = runTest(testToRun, testInfo, true, UrlValidator.ALLOW_ALL_SCHEMES);
		   if(testResult == 1)
		   {
			   numTestsFailed++;
		   }
	   }
	   System.out.println("_____________________________________________________________________________________________________");
	   System.out.println("Authority Partition Test Results:  " + numTestsTotal + " tests run, " + numTestsFailed + " tests failed.");
	   System.out.println("=====================================================================================================\n");
   }
   
   // Partition testing: Port
   public void testPortsPartition(){
	   int numTestsFailed = 0;
	   int numTestsTotal = 0;
	   int testResult;
	   
	   //Create URL/expected result pairs to test port partition 
	   ResultPair[] partitionTestPort = {
			   new ResultPair(":10000", true),
			   new ResultPair(":1", true),
			   new ResultPair(":0", false),
			   new ResultPair(":65535", true),
			   new ResultPair(":65536", false),
			   new ResultPair(":", false),
			   new ResultPair(":abc", false),
			   new ResultPair(":256.256", false),
			   new ResultPair(":100abc", false),
			   new ResultPair(":999999999999999", false),
			   new ResultPair(":-1", false),
	   };
	   numTestsTotal = partitionTestPort.length;
	   
	   //Run tests and tally the number of failures
	   System.out.println("\n=====================================================================================================");	 
	   System.out.println("Running Partition Test for Port");
	   System.out.println("_____________________________________________________________________________________________________");
	   
	   for(int i = 0; i < numTestsTotal; i++){
		   //Create new URL with selected port, and validate.
		   ResultPair testToRun = new ResultPair("http://www.example.com" + partitionTestPort[i].item, partitionTestPort[i].valid);
		   String testInfo = "Port:'" + partitionTestPort[i].item + "'";
		   testResult = runTest(testToRun, testInfo, true, UrlValidator.ALLOW_ALL_SCHEMES);
		   if(testResult == 1)
		   {
			   numTestsFailed++;
		   }
	   }
	   System.out.println("_____________________________________________________________________________________________________");
	   System.out.println("Port Partition Test Results:  " + numTestsTotal + " tests run, " + numTestsFailed + " tests failed.");
	   System.out.println("=====================================================================================================\n");
   }
   
   // Partition testing: Path
   public void testPathPartition(){
	   int numTestsFailed = 0;
	   int numTestsTotal = 0;
	   int testResult;
	   
	   //Create URL/expected result pairs to test path partition 
	   ResultPair[] partitionTestPath = {
			   new ResultPair("/test", true),
			   new ResultPair("/test/testfile", true),
			   new ResultPair("/", true),
			   new ResultPair("", true),
			   new ResultPair("//", false),
			   new ResultPair("//test/testfile", false),
			   new ResultPair("test/testfile", false)
	   };
	   numTestsTotal = partitionTestPath.length;
	   
	   //Run tests and tally the number of failures
	   System.out.println("\n=====================================================================================================");	 
	   System.out.println("Running Partition Test for Path");
	   System.out.println("_____________________________________________________________________________________________________");
	   
	   for(int i = 0; i < numTestsTotal; i++){
		   //Create new URL with selected port, and validate.
		   ResultPair testToRun = new ResultPair("http://www.example.com" + partitionTestPath[i].item, partitionTestPath[i].valid);
		   String testInfo = "Path:'" + partitionTestPath[i].item + "'";
		   testResult = runTest(testToRun, testInfo, true, UrlValidator.ALLOW_ALL_SCHEMES);
		   if(testResult == 1)
		   {
			   numTestsFailed++;
		   }
	   }
	   System.out.println("_____________________________________________________________________________________________________");
	   System.out.println("Path Partition Test Results:  " + numTestsTotal + " tests run, " + numTestsFailed + " tests failed.");
	   System.out.println("=====================================================================================================\n");
   }
   
// Partition testing: Query
   public void testQueryPartition(){
	   int numTestsFailed = 0;
	   int numTestsTotal = 0;
	   int testResult;
	   
	   //Create URL/expected result pairs to test query partition 
	   ResultPair[] partitionTestQuery = {
			   new ResultPair("?param=value", true),
			   new ResultPair("?param1=value1&param2=value2", true),
			   new ResultPair("?", true),
			   new ResultPair("?param", true),
			   new ResultPair("", true),
			   new ResultPair("??", false),
			   new ResultPair("?param=value&&", false),
			   new ResultPair("?param1=value1?param2=value2", false) 
	   };
	   numTestsTotal = partitionTestQuery.length;
	   
	   //Run tests and tally the number of failures
	   System.out.println("\n=====================================================================================================");	 
	   System.out.println("Running Partition Test for Query");
	   System.out.println("_____________________________________________________________________________________________________");
	   
	   for(int i = 0; i < numTestsTotal; i++){
		   //Create new URL with selected query, and validate.
		   ResultPair testToRun = new ResultPair("http://www.example.com" + partitionTestQuery[i].item, partitionTestQuery[i].valid);
		   String testInfo = "Query:'" + partitionTestQuery[i].item + "'";
		   testResult = runTest(testToRun, testInfo, true, UrlValidator.ALLOW_ALL_SCHEMES);
		   if(testResult == 1)
		   {
			   numTestsFailed++;
		   }
	   }
	   System.out.println("_____________________________________________________________________________________________________");
	   System.out.println("Query Partition Test Results:  " + numTestsTotal + " tests run, " + numTestsFailed + " tests failed.");
	   System.out.println("=====================================================================================================\n");
   }
   
   
   public int runTest(ResultPair url, String testInfo, boolean showResult, long schemeOption){
	   String validatorSetting = "";
	   
	   // default validator setting
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   
	   //Can set validator differently
	   if (schemeOption == 2) 
	   { 
		   urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_2_SLASHES);
		   validatorSetting = " Option = ALLOW_2_SLASHES";
	   }
	   if (schemeOption == 3) 
	   { 
		   urlVal = new UrlValidator(null, null, UrlValidator.NO_FRAGMENTS);
		   validatorSetting = " Option = NO_FRAGMENTS";
	   }
	   if (schemeOption == 4) 
	   { 
		   urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_LOCAL_URLS);
		   validatorSetting = " Option = ALLOW_LOCAL_URLS";
	   }
	   	   
	   if(urlVal.isValid(url.item) == url.valid)
	   {
		   return 0;
	   }
	   else
	   {
		   String report = "TEST FAILED: URL='" + url.item + "', Expected:'" + url.valid + "' .isValid():'" + !url.valid + "', " + testInfo;
		   if (showResult) { 
			   System.out.println(report + validatorSetting); 
		   }
		   return 1;
	   }  
   }
   
   public void testIsValid()
   {
	   for(int i = 0;i<10000;i++)
	   {
		   
	   }
   }
   
   public void testAnyOtherUnitTest()
   {
	   
   }
   /**
    * Create set of tests by taking the testUrlXXX arrays and
    * running through all possible permutations of their combinations.
    *
    * @param testObjects Used to create a url.
    */
   

}
