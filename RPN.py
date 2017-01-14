#! /usr/bin/env python

import re

DEBUG = False

def splitter(s):
	ret = re.split("(\+|\-|\*|\/|\>|\<|\=|\(|\)|\,|\%|\:)", s)
	rret = []
	tmp = []
	found = False
	for r in ret:
		if len(r) > 0 and r[0] == '"' and r[-1] != '"' and not found:
			tmp.append(r)
			found = True
		elif len(r) > 0 and r[-1] == '"' and found:
			tmp.append(r)
			rret.append("".join(tmp))
			tmp = []
			found = False
		elif found:
			tmp.append(r)
		else:
			rret.append(r)
	return rret



operands = ["+", "-", "*", "/", ">", "<", "=", "%", ":"]

associativity = {'+': False, '-': False, '*': False, "<": False, ">": False, "/": False, "%": False, ":": False} #False = left, True = right
precedence = {'+': 2, '-': 2, '*': 3, '<': 2, ">": 2, "/":2, "%":2, ":":2}


class Token:
	def __init__(self, text):
		self.text = text
		self.argCount = 0
	def __str__(self):
		return self.text
	def __repr__(self):
		return self.text
	def isdigit(self):
		return self.text.isdigit() or (len(self.text) > 0 and self.text[0] == '"')
	def __len__(self):
		return len(self.text)

def peek(l):
	if (len(l) == 0):
		return None
	else:
		return l[len(l) - 1]

def get_rpn(test, variables, functions, operations, types):
	test = splitter(test)
	functions = [a+"."+b for a in variables for b in operations.keys()] + functions + types.keys()
	if DEBUG:
		print "RPN", test
	i = 0
	out = []
	stack = []
	while (i < len(test)):
		# Read a token.
		tok = Token(test[i])
		# Lookahead
		tok3 = None
		if (i + 2 < len(test)):
			tok3 = Token(test[i + 2])

		# If the token is a number, then add it to the output queue.
		if (tok.isdigit() or tok.text in variables):
			out.append(tok)
		# If the token is a function token, then push it onto the stack.
		elif (tok.text in functions):
			stack.append(tok)
			# Check if the function has no arguments
			if (tok3.text is ')'):
				tok.argCount = 0
			else:
				tok.argCount = 1
		# If the token is a function argument separator (e.g., a comma):
		elif (tok.text is ','):
			# Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue. If no left parentheses are encountered, either the separator was misplaced or parentheses were mismatched.
			# for o in stack:
			k = len(stack) - 1
			while (k >= 0):
				o = stack[k]
				if o.text in functions:
					o.argCount += 1
					break
				k -= 1
			while (peek(stack).text != '('):
				out.append(stack.pop())
		# If the token is an operator, o1, then:
		elif tok.text in operands:
			# while there is an operator token, o2, at the top of the operator stack, and either
			hasOp = len(stack) > 0
			if (hasOp):
				op2 = peek(stack)
				hasOp = op2.text in operands # check if is an operator
			# o1 is left-associative and its precedence is less than or equal to that of o2,
			# or
			# o1 is right associative, and has precedence less than that of o2,
			if (hasOp):
				cond1 = not associativity[tok.text] and precedence[tok.text] <= precedence[op2.text]
				cond2 = associativity[tok.text] and precedence[tok.text] < precedence[op2.text]
				while (hasOp and (cond1 or cond2)):
					#then pop o2 off the operator stack, onto the output queue;
					out.append(stack.pop())
					hasOp = len(stack) > 0
					if (hasOp):
						op2 = peek(stack)
						hasOp = op2.text in operands #check if is an operator
					if (hasOp):
						cond1 = not associativity[tok.text] and precedence[tok.text] <= precedence[op2.text]
						cond2 = associativity[tok.text] and precedence[tok.text] < precedence[op2.text]
			# push o1 onto the operator stack.
			stack.append(tok)
		# If the token is a left parenthesis (i.e. "("), then push it onto the stack.
		elif (tok.text is '('):
			stack.append(tok)
		# If the token is a right parenthesis (i.e. ")"):
		elif (tok.text is ')'):
			# Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
			while (True):
				if (len(stack) > 0):
					tok = peek(stack)
					if (tok.text is '('):
						# Pop the left parenthesis from the stack, but not onto the output queue.
						stack.pop()
						break
					else:
						out.append(stack.pop())
				else:
					break
			# If the token at the top of the stack is a function token, pop it onto the output queue.
			if (len(stack) > 0):
				tok = peek(stack)
				if (tok.text in functions):
					out.append(stack.pop())

		i += 1

	while (len(stack) > 0):
		out.append(stack.pop())

	# print (out, stack, fstack, fpointer)
	if DEBUG:
		print "ret",out
	return out

