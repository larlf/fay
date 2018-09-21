using fay.compiler.lexical;
using fay.lib;
using Mirage.Sys;
using Newtonsoft.Json;
using System;

namespace fayTest
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");

			Token[] tokens = new Token[] {
			};

			int pos = 0;
			Token t = parseAddSub(tokens, ref pos);

			//string str = Newtonsoft.Json.JsonConvert.SerializeObject(t, Formatting.Indented);
			string str = t.ToString();
			Log.Debug(str);

		}

		static Token parseAddSub(Token[] tokens, ref int pos)
		{
			var item1 = parseMulDev(tokens, ref pos);
			if (item1 != null)
			{
				for (; ; )
				{
					if (pos >= tokens.Length)
						break;

					var op = tokens[pos++];
					if (op.Text == "+" || op.Text == "-")
					{
						var item2 = parseMulDev(tokens, ref pos);
						if (item2 != null)
						{
							item1 = new Token(TokenType.OP, op.Text);
						}
					}
					else
					{
						pos--;
						break;
					}
				}

			}

			return item1;
		}

		static Token parseMulDev(Token[] tokens, ref int pos)
		{
			var item1 = parseNot(tokens, ref pos);
			if (item1 != null)
			{
				if (pos >= tokens.Length)
					return item1;

				var op = tokens[pos++];
				if (op.Text == "*" || op.Text == "/")
				{
					var item2 = parseNot(tokens, ref pos);
					if (item2 != null)
					{
						return new Token(TokenType.OP, op.Text);
					}
				}
				else
				{
					pos--;
				}
			}

			return item1;
		}

		static Token parseNot(Token[] tokens, ref int pos)
		{
			if (tokens[pos].Text == "!")
			{
				pos++;
				var item = parseItem(tokens, ref pos);

				if (item != null)
				{
					return new Token(TokenType.OP, "!");
				}
			}
			else
			{
				return parseItem(tokens, ref pos);
			}

			return null;
		}

		static Token parseItem(Token[] tokens, ref int pos)
		{
			if (tokens[pos].Text == "(")
			{
				pos++;
				Token item = parseAddSub(tokens, ref pos);

				if (tokens[pos].Text == ")")
					pos++;

				return item;
			}

			if (tokens[pos].Type == TokenType.Number)
				return tokens[pos++];

			if (tokens[pos].Type == TokenType.ID)
				return tokens[pos++];

			return null;
		}


		static void parseNumber()
		{

		}
	}
}
