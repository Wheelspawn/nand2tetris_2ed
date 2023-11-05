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
        while ("/**" in self.data):
            i = self.data.find("/**")
            j = self.data.find("*/")
            self.data = self.data[:i] + self.data[(j+2):]

        while ("//" in self.data):
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

    def hasMoreTokens(self):
        return (self.i < len(self.tokens))
    
    def advance(self):
        self.currentToken = self.tokens[self.i]
        self.i += 1
        return self.currentToken

    def getCurrentToken(self):
        return self.currentToken

    def getNextToken(self):
        if (self.i+i < len(self.tokens)):
            return self.tokens[self.i+1]
        else:
            return ""
    
    def tokenType(self):
        if self.currentToken in keywords:
            return TokenType.KEYWORD
        if re.match(r'[{}()[\].,;+\-*/&|<>=~]',self.currentToken):
            return TokenType.SYMBOL
        if re.match(r'\d+',self.currentToken):
            return TokenType.INT_CONST
        if re.match(r'"([^"\n]*)"', self.currentToken):
            return TokenType.STRING_CONST
        if re.match(r'\b[a-zA-Z_]\w*\b', self.currentToken):
            return TokenType.IDENTIFIER
        
        return TokenType.UNKNOWN
        
    def keyWord(self):
        if self.tokenType() == TokenType.KEYWORD:
            return list(KeyWord)[keywords.index(self.currentToken)]
        else:
            print("keyword is not of type TokenType.KEYWORD")
    
    def symbol(self):
        if self.tokenType() == TokenType.SYMBOL:
            return self.currentToken
        else:
            print("keyword is not of type TokenType.SYMBOL")
    
    def identifier(self):
        if self.tokenType() == TokenType.IDENTIFIER:
            return self.currentToken
        else:
            print("keyword is not of type TokenType.IDENTIFIER")
    
    def intVal(self):
        if self.tokenType() == TokenType.INT_CONST:
            return int(self.currentToken)
        else:
            print("keyword is not of type TokenType.INT_CONST")
    
    def stringVal(self):
        if self.tokenType() == TokenType.STRING_CONST:
            return self.currentToken
        else:
            print("keyword is not of type TokenType.STRING_CONST")

    
class CompilationEngine():

    inp = None
    outp = None
    tokenizer = None
    
    def __init__(self, inp, outp):

        self.outp = open(outp, 'w')
        self.tokenizer = JackTokenizer(inp)
        self.tokenizer.tokenize()

        self.compileClass()

        self.outp.close()

    def process(self, s, increment=0):
        if (s == self.tokenizer.getCurrentToken()):
            self.outp.write("<" + tokenizer.tokenType() + "> ")
            self.outp.write(tokenizer.getCurrentToken())
            self.outp.write(" </" + tokenizer.tokenType() + ">")
            self.outp.write("\n")
        else:
            print(s)
            print("syntax error")

        self.tokenizer.advance()

    def write(self, s):
        print(s)
        self.outp.write(s)
    
    def compileClass(self):
        self.outp.write("<class>\n")
        self.process("class")
        self.outp.write(self.tokenizer.getCurrentToken())
        self.process("{")

        while (self.tokenizer.getCurrentToken() == "static" or self.tokenizer.getCurrentToken() == "field"):
            compileClassVarDec()

        self.process("}")
            
    def compileClassVarDec(self):
        if (self.tokenizer.getCurrentToken() == "static"):
            self.process("static")
            self.process(self.tokenizer.getCurrentToken())
            while (self.tokenizer.getNextToken() == ","):
                self.process(",")
                self.process(self.tokenizer.getNextToken())
            self.process(";")
        elif (self.tokenizer.getCurrentToken() == "field"):
            self.process("field")
            self.process(self.tokenizer.getCurrentToken())
            while (self.tokenizer.getNextToken() == ","):
                self.process(",")
                self.process(self.tokenizer.getNextToken())
        else:
            pass
    
    def compileSubroutine(self):
        pass
    
    def compileParameterList(self):
        pass
    
    def compileSubroutineBody(self):
        pass
    
    def compileVarDec(self):
        pass
    
    def compileStatements(self):
        pass
    
    def compileLet(self):
        pass
    
    def compileIf(self):
        pass
    
    def compileWhile(self):
        pass
    
    def compileDo(self):
        pass
    
    def compileReturn(self):
        pass
    
    def compileExpression(self):
        pass
    
    def compileTerm(self):
        pass
    
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
            
            
