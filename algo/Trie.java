import java.util.*;
// import java.lang.*;
// import java.io.*;

public class Trie{
	
	static class TrieNode{
		TrieNode[] children = new TrieNode[26];
		boolean isEndOFWord;
		static Map<Integer, String> mp;
 		TrieNode(){
			for (int i = 0; i < 26; i ++){
				children[i] = null;
			}
			isEndOFWord = false;
		} 
		void insert(String s, int indx){
			int letter = s.charAt(indx) - 'a';
			if(this.children[letter] == null) this.children[letter] = new TrieNode();
			if(indx == s.length() - 1){
				this.children[letter].isEndOFWord = true;
				
				return;
			}
			this.children[letter].insert(s, indx + 1);
		}
		
		TrieNode check(String s, int index){
			TrieNode node = this.children[s.charAt(index) - 'a'];
			if(node == null || index == s.length() - 1){
				return node;
			}
			return node.check(s, index + 1);
		}

		Map<Integer, String> ls(String s){
			mp = new HashMap<Integer, String>();
			TrieNode node = this.check(s, 0);
			if(node != null){
				if(node.isEndOFWord) mp.put(mp.size() + 1, s);
				node.dfs(s);
			}
			return mp;
		}

		void dfs(String s){
			for (int i = 0; i < 26; i ++){
				String curS = s;
				if(this.children[i] != null){
					char ch = 'a';
					ch += i;
					if(this.children[i].isEndOFWord == true){
						mp.put(mp.size() + 1, curS + ch);
					}
					if(this.children[i] != null){
						this.children[i].dfs(curS + ch);
					}
				}
			}
			return;
		}
	}
	public static void main(String[] args){
		Scanner scan = new Scanner(System.in);
		int n = scan.nextInt(), m = scan.nextInt();
		ArrayList<String> words = new ArrayList<String>();
		for (int i = 0; i < n; i ++){
			String s = scan.next();
			words.add(s);
		}

		TrieNode root = new TrieNode();
		Collections.sort(words);
		for (int i = 0; i < n; i ++){
			String s = words.get(i);
			root.insert(s, 0);
		}
		for (int i = 0; i < m; i ++){
			String s = scan.next();
			ls(s, root);
		}
		scan.close();
	}
	static void ls(String s, TrieNode root){
		Map<Integer, String> mp = root.ls(s);
		// System.out.print(mp + " ");
		for (int i = 1; i <= mp.size(); i ++){
			System.out.print(mp.get(i) + " ");
		}
		System.out.println();
	}
}
/*
5 3
ad
a
abc
aboba
b 
3 a
2 ab
1 b
*/
