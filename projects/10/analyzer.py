import os
import re
import sys
import xml.etree.ElementTree as ET

from enum import Enum

class TokenType(Enum):
    KEYWORD = 0
    SYMBOL = 1
    IDENTIFIER = 2
    INT_CONST = 3
    STRING_CONST = 4
    UNKNOWN = 5

class KeyWord(Enum):
    CLASS = 0
    CONSTRUCTOR = 1
    FUNCTION = 2
    METHOD = 3
    FIELD = 4
    STATIC = 5
    VAR = 6
    INT = 7
    CHAR = 8
    BOOLEAN = 9
    VOID = 10
    TRUE = 11
    FALSE = 12
    NULL = 13
    THIS = 14
    LET = 15
    DO = 16
    IF = 17
    ELSE = 18
    WHILE = 19
    RETURN = 20
    
keywords = ["class","constructor","function",
            "method","field","static","var",
            "int","char","boolean","void",
            "true","false","null","this","let",
            "do","if","else","while","return"]
symbols = ["{","}","(",")","[","]",".",",",";","+","-","*","/","&","|","<",">","=","~"]
class_var_decs = ["static","field"]
types = ["int","char","boolean"]
subroutine_decs = ["constructor", "function", "method"]
statements = ["let", "if", "while", "do", "return"]
ops = ["+", "-", "*", "/", "&", "|", "<", ">", "="]
unary_ops = ["-", "~"]
keyword_constants = ["true", "false", "null", "this"]

class JackTokenizer():
    
    data = None
    tokens = []
    currentToken = ''
    i = 0
    
    def __init__(self, file):
        with open(file, 'r') as f:

            # read file
            self.data = f.read()

    def tokenize(self):
        
        # remove comments
        while "/**" in self.data:
            i = self.data.find("/**")
            j = self.data.find("*/")
            self.data = self.data[:i] + self.data[(j+2):]

        while "//" in self.data:
            i = self.data.find("//")
            j = self.data[(i+2):].find("\n")
            self.data = self.data[:i] + self.data[(i+j+2):]
            
        # Define a regex pattern to match identifiers, symbols, and string constants
        pattern = re.compile(r'([a-zA-Z_]\w*)|(\d+)|(".*?")|([{}()[\].,;+\-*/&|<>=~])')
        
        # Find all matches in the input string
        tokens = pattern.findall(self.data)
        
        # The result is a list of tuples, we will flatten it and remove empty strings
        tokens = [token for group in tokens for token in group if token]
        
        self.tokens = tokens
        self.currentToken = tokens[0]
        
    def hasMoreTokens(self):
        return self.i < len(self.tokens)
    
    def advance(self):
        self.currentToken = self.tokens[self.i+1]
        self.i += 1
        return self.currentToken

    def getCurrentToken(self):
        return self.currentToken

    def getNextToken(self):
        if self.i+1 < len(self.tokens):
            return self.tokens[self.i+1]
        else:
            return ""
    
    def tokenType(self):
        if self.keyword():
            return "keyword"
        if self.symbol():
            return "symbol"
        if self.intVal():
            return "integerConstant"
        if self.stringVal():
            return "stringConstant"
        if self.identifier():
            return "identifier"
        
        return "UNKNOWN"
    
    def keyword(self):
        return self.currentToken in keywords

    def symbol(self):
        return re.match(r'[{}()[\].,;+\-*/&|<>=~]',self.currentToken)

    def identifier(self):
        return re.match(r'\b[a-zA-Z_]\w*\b', self.currentToken)
    
    def intVal(self):
        return re.match(r'\d+',self.currentToken)

    def stringVal(self):
        return re.match(r'"([^"\n]*)"', self.currentToken)
    
    '''
    def keyword(self):
        if self.tokenType() == TokenType.KEYWORD:
            return list(KeyWord)[keywords.index(self.currentToken)]
        else:
            print("token is not of type TokenType.KEYWORD")
    
    def symbol(self):
        if self.tokenType() == TokenType.SYMBOL:
            return self.currentToken
        else:
            print("token is not of type TokenType.SYMBOL")
    
    def identifier(self):
        if self.tokenType() == TokenType.IDENTIFIER:
            return self.currentToken
        else:
            print("token is not of type TokenType.IDENTIFIER")
    
    def intVal(self):
        if self.tokenType() == TokenType.INT_CONST:
            return int(self.currentToken)
        else:
            print("token is not of type TokenType.INT_CONST")
    
    def stringVal(self):
        if self.tokenType() == TokenType.STRING_CONST:
            return self.currentToken
        else:
            print("token is not of type TokenType.STRING_CONST")'''

class CompilationEngine():

    inp = None
    outp = None
    tokenizer = None
    
    ind = ""
    
    def __init__(self, inp, outp):

        self.outp = open(outp, 'w')
        self.tokenizer = JackTokenizer(inp)
        self.tokenizer.tokenize()

        self.compileClass()

        self.outp.close()

    def process(self, s):
        print(s,"  ~~  ", self.tokenizer.getCurrentToken())
        print()
        
        if s == self.tokenizer.getCurrentToken():
            
            self.write("<{}> {} </{}>\n".format(self.tokenizer.tokenType(),
                                                self.tokenizer.getCurrentToken(),
                                                self.tokenizer.tokenType()),
                                                False)
            '''
            self.write("<" + self.tokenizer.tokenType() + "> " +
                       self.tokenizer.getCurrentToken() 
                       " </" + self.tokenizer.tokenType() + ">\n") '''
        else:
            print("process: syntax error")

        self.tokenizer.advance()

    def write(self, s, indent = True):
        
        inc = ""
        
        if indent:
            if (len(s) > 1) and (s[1] == "/"):
                self.ind = self.ind[:-2]
            else:
                inc = "  "
        
        self.outp.write(self.ind + s)
        
        self.ind += inc

    def compileIntegerConstant(self):
        if (self.tokenizer.intVal()):
            self.process(self.tokenizer.getCurrentToken())
        else:
            print("compileIdentifier: syntax error")

    def compileStringConstant(self):
        if (self.tokenizer.stringVal()):
            self.process(self.tokenizer.getCurrentToken())
        else:
            print("compileIdentifier: syntax error")
    
    def compileKeyword(self):
        if self.tokenizer.keyword():
            self.process(self.tokenizer.getCurrentToken())
        else:
            print("compileKeyword: syntax error")
            
    def compileSymbol(self):
        if self.tokenizer.symbol():
            self.process(self.tokenizer.getCurrentToken())
        else:
            print("compileSymbol: syntax error")

    def compileIdentifier(self):
        if (self.tokenizer.identifier()):
            self.process(self.tokenizer.getCurrentToken())
        else:
            print("compileIdentifier: syntax error")
    
    def compileType(self):
        if self.tokenizer.getCurrentToken() in types:
            self.compileKeyword()
        elif self.tokenizer.identifier():
            self.compileIdentifier()
        else:
            print("compileType: syntax error")
                
    def compileClass(self):
        print(self.tokenizer.tokens)
        
        self.write("<class>\n")
        
        self.compileKeyword() # class
        self.compileIdentifier() # class name
        self.compileSymbol() # {

        while self.tokenizer.getCurrentToken() in class_var_decs:
            self.compileClassVarDec()

        while self.tokenizer.getCurrentToken() in subroutine_decs:
            self.compileSubroutine()

        self.compileSymbol() # "}"
        
        self.write("</class>")
        
    def compileClassVarDec(self):
        
        self.write("<classVarDec>\n")
        
        if self.tokenizer.getCurrentToken() in class_var_decs:
            self.compileKeyword()
            self.compileKeyword()
        else:
            print("compileClassVarDec: syntax error")
            
        while (self.tokenizer.getNextToken() == ","):
            self.compileIdentifier()
            self.compileSymbol() # ,
        self.compileIdentifier()
        self.compileSymbol() # ;
        
        self.write("</classVarDec>\n")
    
    def compileSubroutine(self):
        
        self.write("<subroutineDec>\n")
        
        if self.tokenizer.getCurrentToken() in subroutine_decs:
            self.compileKeyword()
        else:
            print("compileSubroutine: syntax error")

        self.compileType() # type
        self.compileIdentifier() # subroutine name
        
        self.compileSymbol() # (
        self.compileParameterList()
        self.compileSymbol() # )
        
        self.compileSubroutineBody()
        
        self.write("</subroutineDec>\n")
    
    def compileParameterList(self):
        
        self.write("<parameterList>\n")
        
        if self.tokenizer.getCurrentToken() != ")":
            self.compileType() # type
            self.compileIdentifier()
            
            while self.tokenizer.getCurrentToken() != ")":
                self.compileSymbol() # ,
                self.compileType() # type
                self.compileIdentifier()
        
        self.write("</parameterList>\n")
    
    def compileSubroutineBody(self):
        
        self.write("<subroutineBody>\n")
        
        self.compileSymbol() # {
        
        while (self.tokenizer.getCurrentToken() == "var"):
            self.compileVarDec()
        
        self.compileStatements()
        
        self.compileSymbol() # }
        
        self.write("</subroutineBody>\n")
    
    def compileVarDec(self):
        
        self.write("<varDec>\n")
        
        self.compileKeyword() # var
        self.compileType()
        self.compileIdentifier()
        
        while self.tokenizer.getCurrentToken() == ",":
            self.compileSymbol() # ,
            self.compileIdentifier()
        
        self.compileSymbol() # ;
        
        self.write("</varDec>\n")
    
    def compileStatements(self):
        
        self.write("<statements>\n")
        
        statement = self.tokenizer.getCurrentToken()
        while statement in statements:
            match statement:
                case "let":
                    self.compileLet()
                case "if":
                    self.compileIf()
                case "while":
                    self.compileWhile()
                case "do":
                    self.compileDo()
                case "return":
                    self.compileReturn()
                
            statement = self.tokenizer.getCurrentToken()
        
        self.write("</statements>\n")
    
    def compileLet(self):
        pass
    
    def compileIf(self):
        pass
    
    def compileWhile(self):
        pass
    
    def compileDo(self):
        pass
    
    def compileReturn(self):
        
        self.write("<returnStatement>\n")
        
        if self.tokenizer.getCurrentToken() == "return":
            self.compileKeyword() # return
            if self.tokenizer.getCurrentToken() != ";":
                self.compileExpression()
            self.compileSymbol() # ;
        else:
            print("compileReturn: syntax error")
        
        self.write("</returnStatement>\n")
    
    def compileExpression(self):
        
        self.write("<expression>\n")
        
        self.compileTerm()
        if self.tokenizer.getCurrentToken() in ops:
            self.compileSymbol()
            self.compileTerm()
        
        self.write("</expression>\n")
    
    def compileTerm(self):
        
        self.write("<term>\n")
        
        statement = self.tokenizer.tokenType()
        match statement:
            case "integerConstant":
                self.compileIntegerConstant()
            case "stringConstant":
                self.compileStringConstant()
            case "keyword":
                self.compileKeyword()
            case "identifier":
                self.compileIdentifier()
        
        self.write("</term>\n")
        
    def compileExpressionList(self):
        pass
    
    
if __name__ == '__main__':
    
    arg = sys.argv[1]
    # print(os.path.isdir(arg))
    # print(arg.lower().endswith('.jack'))
    
    if os.path.isdir(arg):
        
        for inp in os.listdir(arg):

            if inp.endswith('.jack'):

                outp = inp[:-4] + 'hack'
                compiler = CompilationEngine(inp,outp)
    
    else:
        if arg.endswith('.jack'):

            # arg is jack file
            outp = arg[:-4] + 'new.xml'
            compiler = CompilationEngine(arg,outp)
            
        else:
            print(".jack file was not found")
            
            
