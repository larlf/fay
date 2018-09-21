using fay.lib;
using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// 抽象语法树的节点
	/// </summary>
	public abstract class AstNode
	{
		protected string text;  //节点的文字
		protected AstNode parent = null;  //父节点
		protected List<AstNode> nodes = new List<AstNode>();  //子节点

		public string Text { get => text; }
		public List<AstNode> ChildNodes { get => nodes; }

		public AstNode(string text)
		{
			this.text = text;
		}

		/// <summary>
		/// 子节点的数量
		/// </summary>
		public int ChildNodeNum
		{
			get
			{
				return this.nodes.Count;
			}
		}

		virtual public FayDataType ValueType()
		{
			return FayDataType.Void;
		}

		/// <summary>
		/// 添加子节点
		/// </summary>
		/// <param name="node"></param>
		public void AddChildNode(AstNode node)
		{
			//可能会添加空节点
			if (node != null)
				node.parent = this;

			this.nodes.Add(node);
		}

		/// <summary>
		/// 检查合法性，子节点可重载实现
		/// </summary>
		virtual public void Check()
		{
			foreach (var it in this.nodes)
			{
				if (it != null)
					it.Check();
			}
		}

		/// <summary>
		/// 生成代码
		/// </summary>
		/// <param name="builder"></param>
		virtual public void Build(CodeBuilder builder)
		{
			foreach (var it in this.nodes)
			{
				if (it != null)
					it.Build(builder);
			}
		}

		/// <summary>
		/// 生成格式化的代码
		/// </summary>
		/// <param name="indent"></param>
		/// <returns></returns>
		public virtual void Format(StringBuilder sb, string indent = "")
		{
			foreach (var it in this.nodes)
			{
				if (it != null)
				{
					it.Format(sb, indent);
				}
			}
		}

		/// <summary>
		/// 打印有缩进的信息
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public virtual string ToString(string index)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(index);
			sb.Append(this.GetType().Name);
			sb.Append("(");
			sb.Append(this.text);
			sb.Append(")");

			foreach (var it in this.nodes)
			{
				if (it != null)
				{
					sb.Append("\n");
					sb.Append(it.ToString(index + "    "));
				}
			}

			return sb.ToString();
		}

		/// <summary>
		/// 重载信息打印
		/// </summary>
		/// <returns></returns>
		public override string ToString()
		{
			return this.ToString("");
		}


	}
}
