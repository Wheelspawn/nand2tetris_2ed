import sys
import os

import xml.etree.ElementTree as ET

from enum import Enum

class TokenType(Enum):
    KEYWORD = 1
    SYMBOL = 2
    IDENTIFIER = 3
    INT_CONST = 4
    STRING_CONST = 5
    UNKNOWN = 6

keywords = ["class","constructor","function",
            "method","field","static","var",
            "int","char","boolean","void",
            "true","false","null","this","let",
            "do","if","else","while","return"]

symbols = ["{","}","(",")","[","]",".",",",";","+","-","*","/","&","|","<",">","=","~"]

class JackTokenizer():
    
    token = ""
    
    def __init__(self, file):
        with open(file, 'r') as f:
            data = f.read().replace('\n', '')
            print(data)
            
    def hasMoreTokens(self):
        pass
    
    def advance(self):
        pass
    
    def tokenType(self):
        if self.token in keywords:
            return TokenType.KEYWORD
        if self.token in symbols:
            return TokenType.SYMBOL
        if self.token.isnumeric():
            return TokenType.INT_CONST
        if self.token[0] == "\"" and self.token[-1] == "\"":
            return TokenType.STRING_CONST
        
    
    def keyWord(self):
        pass
    
    def symbol(self):
        pass
    
    def identifier(self):
        pass
    
    def intVal(self):
        pass
    
    def stringVal(self):
        pass

    
class CompilationEngine():
    def __init__(self):
        pass
    
    def compileClass(self):
        pass
    
    def compileClassVarDec(self):
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
        
        for file in os.listdir(arg):
            
            if file.lower().endswith('.jack'):
                
                tokenizer = JackTokenizer(file)
    
    else:
        if arg.lower().endswith('.jack'):
            
            tokenizer = JackTokenizer(arg)
            
        else:
            print(".jack file was not found")
            
            