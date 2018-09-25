using fay.compiler.ast;
using fay.compiler.lexical;
using fay.compiler.runtime;
using fay.compiler.syntax;
using Mirage.Sys;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.compiler
{
	namespace FayC
	{
		class Program
		{
			static void Main(string[] args)
			{
				//设置Log的处理方式
				Log.SetAppender(new ColorLogAppender());

				Log.Debug("Current path : " + Directory.GetCurrentDirectory());

				//没参数返回
				if (args.Length <= 0)
					return;

				string filename = Path.Combine(Directory.GetCurrentDirectory(), args[0]);
				Log.Debug("Complate program file : " + filename);

				try
				{
					string text = Mirage.Utils.FileUtils.ReadFile(filename);

					//语法解析，生成token列表
					Lexer lexer = Lexer.GetInstance();
					List<Token> r = lexer.Execute(text);

					//用于检查Token
					//foreach (var it in r)
					//	Log.Debug(it);

					//生成AST
					Parser parser = new Parser();
					AstNode ast = parser.Execute(r, filename);

					Log.Debug("[AST]\n"+ast);

					StringBuilder sb = new StringBuilder();
					ast.Format(sb);
					Console.WriteLine(sb);

					//ast.Check();
					//Log.Debug("[AST]\n" + ast);

					//编译
					CodeBuilder builder = new CodeBuilder();
					FayLib lib = new FayLib(builder.domain);
					builder.bindLib(lib);
					ast.Build(builder);

					Console.WriteLine("********************");

					builder.build1();
					Console.WriteLine(lib.ToString());

					//生成库文件
					//FayLib lib = new FayLib();
					//lib.Name = "TestLib";
					//lib.Major = 0;
					//lib.Minor = 1;
					//lib.ParseFile(ast);

					//if(lib!=null)
					//{
					//	Log.Debug("\n" + lib);

					//	MemoryStream ms = new MemoryStream();******************************************
					//	lib.ToData(ms);
					//	IOUtils.WriteFile("e:/temp/fay.lib", ms.ToArray());
					//}
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex);
				}

			}
		}
	}
}


