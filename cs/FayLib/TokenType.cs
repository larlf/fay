namespace fay.lib
{
	/// <summary>
	/// Token的类型
	/// </summary>
	public enum TokenType
	{
		None=0,

		Assign = '=',
		Colon = ':',
		Semicolon = ';',
		Comma = ',',
		LeftBrace = '{',
		RightBrace = '}',

		OP=256,  //让默认的值都在256以为，为字符空出位置

		//注释
		DoubleSlash, Comment,

		//关键字
		Using, Package, Var, Function, Class, Interface, DescSymbol, SystemName,
		If, Else, ElseIf, For, Foreach, While, Do, Return,

		//数据和类型
		ID, BasicType, Number, String, Char
	}
}
