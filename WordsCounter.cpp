using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Xml;
namespace WordsCounter
{
	class Program
	{

		public static void DownloadFile()
		{
			if (System.Net.NetworkInformation.NetworkInterface.GetIsNetworkAvailable())
			{
				using (System.Net.WebClient client = new System.Net.WebClient())
				{
					client.DownloadFile("http://www.gutenberg.org/files/2701/2701-0.txt","D:\\Moby_Dick.txt");
					Console.WriteLine("File Downloaded");
				}
			}
		}// End of DownloadFile method
	
		public static void CreateXml()		
		{
			XmlWriter writer = XmlWriter.Create(@"D:\Words.xml");
			writer.WriteStartDocument();
			writer.WriteComment("This File is Generated to Count the Words in Moby Dick Book.");
			writer.WriteStartElement("Words");
			writer.WriteEndDocument();
			writer.Close();
			
		}	// End of CreateXml method	
		
		public static void CountWordsOccurrences(int v, string k)
		{
			
			//create new instance of XmlDocument
			XmlDocument doc = new XmlDocument();

			//load from file
			doc.Load(@"D:\Words.xml");
			
			//select the specific node ..              
			XmlElement el = (XmlElement)doc.SelectSingleNode("/Words/Word"); 
			
			//create node and add its value and attributes
			XmlElement node = doc.CreateElement("Word");
			node.SetAttribute("text", k);
			node.SetAttribute("count", v.ToString());
			doc.DocumentElement.AppendChild(node);	
			
			//save back
			doc.Save(@"D:\Words.xml");
			
		} // End of CountWordsOccurrences method
	
		static void Main(string[] args)
		{

			// Download the Moby Dick Text Book
			// DownloadFile();
			
			// Read a file into a string (this file must live in the same directory as the executable)
			string filename = "D:\\Moby_Dick.txt";
			string inputText = File.ReadAllText(filename);
			CreateXml();
			// Convert our input to lowercase
			// inputText = inputText.ToLower();        
			
			// Characters to remove from the input text
			string[] removeChars = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
			"“","”","!","?",":",";", ",", ".", "-", "_", "^", "(", ")", "[", "]","\"", "\n", "\t", "\r"};			
			foreach (string character in removeChars)
			{
				inputText = inputText.Replace(character, " ");
			}
			
			// Split words on the text after space and put them into a List of strings
			List<string> wordList = inputText.Split(' ').ToList();
			
			// Define and remove commonWords
			/*
			string[] commonWords = new string[] { "The","and", "the", "it", "for", "this", "but", "to", "on", "of", "\n" };
			foreach (string word in commonWords)
			{
				// Here a wile loop to remove common words that maybe we don't want to show them in
				// our list of words, "prepositions and stop words"
				while ( wordList.Contains(word) )
				{
					wordList.Remove(word);
				}
			}
			*/
			// Create a new Dictionary object called "wordsRepository"
			Dictionary<string, int> wordsRepository = new Dictionary<string, int>();
			
			// Loop over all over the words in the wordList...
			foreach (string word in wordList)
			{
				// If the length of the word is at least two letters...
				// We can change this condition according to what we want
				if (word.Length >= 2) 
				{
					// Check if the wordsRepository already has the word.
					if ( wordsRepository.ContainsKey(word) )
					{
						// If this word appeared before, increment the counter for how many times it appears
						wordsRepository[word]++;
					}
					else
					{
						// else, if it's a new word then add it to the wordsRepository with an initial counter of 1
						wordsRepository[word] = 1;
					}
					
				} 
				
			}
			
			// Create a wordsRepository sorted by counter(value)
			var sortedWord = (from entry in wordsRepository orderby entry.Value descending select entry).ToDictionary(term => term.Key, term => term.Value);
			
			// Loop through the wordsRepository and output and save the words on the Xml Document
			int count = 1;
			Console.WriteLine("---- Most Repeated Words in the File: " + filename + " ----");
			Console.WriteLine();
			Console.WriteLine("----Counting Takes Some Time if the File is Too Big----");
			Console.WriteLine();
			Console.WriteLine("----Start Counting----");
			foreach (KeyValuePair<string, int> term in sortedWord)
			{
				// Output the most repeated words and its counts
				Console.WriteLine(count + "\t" + term.Key + "\t" + term.Value);
				count++;
				// Call the Function of Words Counter!			
				CountWordsOccurrences(term.Value, term.Key);				
			}
			
			Console.WriteLine();
			Console.WriteLine("----Counting Completed----");
			// Wait for the user to press a key before exiting
			Console.ReadKey();		
			
		} // End of Main method
		
	} // End of Program class
	
} // End of WordsCounter namespace
