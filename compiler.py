#! /usr/bin/env python

import sys
sys.path.append("/etc/slope/")
import pprint as pp
from RPN import *
from ctypes import c_uint8, c_uint32, c_int32

DEBUG = False

BLOCKSTART = "{"
BLOCKEND = "}"

DEFBLOCKS = ["function", "while", "if"]

GLOBALNAMES = ["print", "return", "input"]

STRINGS = {}

operands = ["+", "-", "*", "/", ">", "<", "=", "%", ":"]

IDS = {"main":0}
LASTID = 3


OPERATIONS = {}
TYPES = {}

OPCODES = {
"LOAD_STACK" : c_uint8(1),
"STORE_CONST" : c_uint8(2),
"CALL" : c_uint8(3),
"STORE_RES" : c_uint8(4),
"STORE_STACK" : c_uint8(5),
"OUT" : c_uint8(6),
"SUM" : c_uint8(7),
"COPY" : c_uint8(8),
"RET" : c_uint8(9),
"CLEAN_STACK" : c_uint8(10),
"DISCARD_RES" : c_uint8(11),
"COPY_OPERANDS" : c_uint8(12),
"STORE_OP": c_uint8(13),
"LOWER": c_uint8(14),
"JMP_TRUE": c_uint8(15),
"JMP": c_uint8(16),
"MIN": c_uint8(17),
"OP" : c_uint8(18),
"BUILD_TYPE" : c_uint8(19),
"MUL": c_uint8(20),
"DIV": c_uint8(21),
"MOD": c_uint8(22),
"HIGHER": c_uint8(23),
"EQUALS": c_uint8(24),
"IN" : c_uint8(25)
}



def stripline(line):
	return re.sub('\s+(?=([^"]*"[^"]*")*[^"]*$)', "", line).replace("\t","").replace("\n","")


def strip(lines):
	if type(lines) is str:
		return stripline(lines)
	for i in range(0, len(lines)):
		lines[i] = stripline(lines[i].replace(" ","").replace("\t","").replace("\n",""))
	return lines

def block_start(line):
	for BLOCK in DEFBLOCKS:
		if line[0:len(BLOCK)] == BLOCK:
			return [BLOCK, line[len(BLOCK):line.find(BLOCKSTART)]]
	return False


def handle_expr(expr, vars, names, ops, types):
	global LASTID
	global STRINGS
	ret = get_rpn(expr, vars, names, ops, types)
	for i in range(0, len(ret)):
		if ret[i].text is not None and len(ret[i].text) > 0 and ret[i].text[0] == '"':
			if ret[i].text not in STRINGS:
				STRINGS[ret[i].text] = LASTID
				LASTID = LASTID + 1
	return ret


def parse(lines):
	stack = []
	varstack = []
	code = []
	count = 0
	countstack = []
	for rawline in lines:
		line = strip(rawline)
		bs = block_start(line)
		if bs != False:
			stack.append(bs)
			code.append(['prologue', bs])
			countstack.append(count)
			if bs[0] == "function":
				sign = bs[1]
				name = sign[0:sign.find("(")]
				GLOBALNAMES.append(name)
				args = sign[sign.find("(")+1:sign.find(")")].split(",")
				if args == [""]:
					args = []
				code[-1].append(args)
				code[-1].append(name)
				for arg in args:
					varstack.append(arg)
				count = len(args)
			else:
				count = 0
			
		elif line == BLOCKEND:
			code.append(['epilogue', stack.pop()])
			while count > 0:
				varstack.pop()
				count -= 1
			count = countstack.pop()
		elif line.find("=") >= 0:
			rside = line.split("=")[0]
			if rside not in varstack:
				varstack.append(rside)
				count += 1
				code.append(['initialize', [rside, handle_expr(line.split("=")[1], varstack, GLOBALNAMES, OPERATIONS, TYPES)]])
			else:
				code.append(['assign', [rside, handle_expr(line.split("=")[1], varstack, GLOBALNAMES, OPERATIONS, TYPES)]])
			
		else:
			code.append(['rpn', handle_expr(line, varstack, GLOBALNAMES, OPERATIONS, TYPES)])
		#print code
	return code
		

def to_code(ops, varnames, funcnames):
	if DEBUG:
		print "TO CODE", ops
	ret = []
	for op in ops:
		isInt = False
		isVar = False
		isOp = False
		isStr = False
		if len(op.text) > 0 and op.text[0] == '"':
			isStr = True
		try:
			int(op.text)
			isInt = True
			#print op.text, "is int"
		except:
			isInt = False
		if op.text in varnames:
			isVar = True
			#print op.text, "is var"
		elif op.text in operands:
			isOp = True
			#print op.text, "is op.text"
		if isStr:
			ret.append(["STORE_CONST", STRINGS[op.text]])
		elif isInt:
			ret.append(["STORE_CONST", int(op.text)])
			ret.append(["STORE_CONST", 1])
			ret.append(["STORE_CONST", 'i'])
			ret.append(["LOAD_STACK", None])
			ret.append(["STORE_RES", None])
		elif isVar:
			ret.append(["STORE_STACK", varnames.index(op.text)-len(varnames)+1])
		elif isOp:
			if op.text == "+":
				ret.append(["SUM", None])
				ret.append(["STORE_RES", None])
			elif op.text == "-":
				ret.append(["MIN", None])
				ret.append(["STORE_RES", None])
			elif op.text == "/":
				ret.append(["DIV", None])
				ret.append(["STORE_RES", None])
			elif op.text == "*":
				ret.append(["MUL", None])
				ret.append(["STORE_RES", None])
			elif op.text == "%":
				ret.append(["MOD", None])
				ret.append(["STORE_RES", None])
			elif op.text == "<":
				ret.append(["LOWER", None])
				ret.append(["STORE_RES", None])
			elif op.text == ">":
				ret.append(["HIGHER", None])
				ret.append(["STORE_RES", None])
			elif op.text == ":":
				ret.append(["EQUALS", None])
				ret.append(["STORE_RES", None])
		elif op.text == "print":
			ret.append(["STORE_CONST", 1])
			ret.append(["OUT", None])
		elif op.text == "input":
			ret.append(["STORE_CONST", 2])
			ret.append(["IN", None])
		elif op.text == "return":
			for i in range(len(varnames)):
				ret.append(["DISCARD_RES", None])
			ret.append(["RET", None])
		elif op.text in GLOBALNAMES:
			ret.append(["call", op.text])
		elif "." in op.text:
			v = op.text.split(".")[0]
			f = op.text.split(".")[1]
			ret.append(["STORE_STACK", varnames.index(v)-len(varnames)+1])
			ret.append(["call", f])
		elif op.text in TYPES:
			ret.append(["BUILD_TYPE", TYPES[op.text]])
	if DEBUG:
		print "ret: ", ret
	return ret



def precomp(parsed):
	varstack = []
	count = 0
	countstack = []
	ret = []
	label = 0
	whilestack = []
	ifstack = []
	for line in parsed:
		#print line
		if line[0] == "prologue":
			countstack.append(count)
			count = 0
			if line[1][0] == "function":
				ret.append(line)
				varstack += line[2]
				count = len(varstack)
			if line[1][0] == "if":
				ret.append(["label", label])
				ret += to_code(handle_expr(line[1][1], varstack, GLOBALNAMES, OPERATIONS, TYPES), varstack, GLOBALNAMES)
				info = [label]
				label += 1
				ret.append(["JMP_TRUE", label])
				ret.append(["JMP", "fine_if"])
				info.append(len(ret)-1)
				ret.append(["label", label])
				label += 1
				ifstack.append(info)
			if line[1][0] == "while":
				ret.append(["label", label])
				ret += to_code(handle_expr(line[1][1], varstack, GLOBALNAMES, OPERATIONS, TYPES), varstack, GLOBALNAMES)
				info = [label]
				label += 1
				ret.append(["JMP_TRUE", label])
				ret.append(["JMP", "fine_while"])
				info.append(len(ret)-1)
				ret.append(["label", label])
				label += 1
				whilestack.append(info)
			
		if line[0] == "epilogue":
			if line[1][0] == "function":
				ret.append(line)
			if line[1][0] == "if":
				info = ifstack.pop()
				ret[info[1]][1] = label
				ret.append(["label", label])
				label += 1
			if line[1][0] == "while":
				info = whilestack.pop()
				ret[info[1]][1] = label
				ret.append(["JMP", info[0]])
				ret.append(["label", label])
				label += 1
			while count > 0:
				varstack.pop()
				count -= 1
				#if line[1][0] != "function":
					#ret.append(["DISCARD_RES", None]) ####### FORSE MALEEEE
			count = countstack.pop()
		if line[0] == "initialize":
			ret += to_code(line[1][1], varstack, GLOBALNAMES)
			ret.append(["STORE_OP", None])
			varstack.append(line[1][0])
			count += 1
		if line[0] == "assign":
			ret += to_code(line[1][1], varstack, GLOBALNAMES)
			ret.append(["STORE_STACK", varstack.index(line[1][0])-len(varstack)+1])
			ret.append(["COPY", None])
		if line[0] == "rpn":
			tc = to_code(line[1], varstack, GLOBALNAMES)
			if tc != []:
				ret += tc
				ret.append(["CLEAN_STACK", None])
	return ret

def comp(precomp):
	ret = []
	for line in precomp:
		if line[0] == "prologue":
			if line[1][0] == "function":
				ret.append(line)
				ret.append(["COPY_OPERANDS", len(line[2])])
		else:
			ret.append(line)
	
	rret = []
	i = 0
	labels = {}
	for line in ret:
		if line[0] == "prologue" or line[0] == "epilogue":
			rret.append(line)
			i = i + 1
		else:
			if line[0] == "label":
				labels[line[1]] = i
			else:
				rret.append(line)
				i += 1

	for i in range(0, len(rret)):
		if rret[i][0] == "JMP" or rret[i][0] == "JMP_TRUE":
			if labels[rret[i][1]] - i - 1 > 0:
				rret[i][1] = labels[rret[i][1]] - i - 1
			else:
				rret[i][1] = labels[rret[i][1]] - i - 1


	return rret


def ass(bytecode):
	global LASTID
	ret = []
	for i in range(0, len(bytecode)):
		if bytecode[i][0] == "prologue" and bytecode[i][1][0] == "function":
			l = 0
			for j in range(i+1, len(bytecode)):
				if bytecode[j][0] == "epilogue" and bytecode[i][1][0] == "function":
					break
				else:
					if bytecode[j][1] == None:
						l += 1
					else:
						l += 5
			ret.append('c')
			if bytecode[i][3] != "main":
				ret.append(LASTID)
				IDS[bytecode[i][3]] = LASTID
				LASTID += 1
			else:
				ret.append(0)
			ret.append(l)
		elif bytecode[i][0] == "epilogue" and bytecode[i][1][0] == "function":
			pass
		elif bytecode[i][0] == "call":
			if bytecode[i][1] in OPERATIONS:
				ret.append(["OP", OPERATIONS[bytecode[i][1]]])
			else:
				ret.append(["CALL", IDS[bytecode[i][1]]])
		else:
			ret.append(bytecode[i])
	
	return ret
			

def tohex(elem):
	if type(elem) is int:
		return c_uint32(elem)
	elif type(elem) is str:
		return c_uint32(ord(elem))

def save(asm, ff):
	f = open(ff, "wb")
	for op in asm:
		if type(op) is list:
			f.write(OPCODES[op[0]])
			if op[0] == "STORE_CONST" or op[0] == "STORE_STACK" or op[0] == "JMP" or op[0] == "JMP_TRUE":
				if type(op[1]) is int:
					f.write(c_int32(op[1]))
				if type(op[1]) is str:
					f.write(c_int32(ord(op[1])))
			elif op[1] is not None:
				f.write(tohex(op[1]))
		else:
			if type(op) == c_uint8:
				f.write(op)
			else:
				f.write(tohex(op))
	f.close()


def add_io():
	r =  ["d", 1, 4, 1]
	r += ["d", 2, 4, 0]
	return r

def add_str():
	ret = []
	global STRINGS
	for k in STRINGS:
		ret.append("s")
		ret.append(STRINGS[k])
		ret.append(len(k)-2)
		for c in k[1:-1]:
			ret.append(c_uint8(ord(c)))
	return ret




showasm = False
if len(sys.argv) > 3:
	if sys.argv[3] == "--show-asm":
		showasm = True

if len(sys.argv) < 2:
	print "Usage ./compiler.py <script> <output> <flags>"
	sys.exit(0)

if len(sys.argv) < 3:
	tos = sys.argv[1]+".out"
else:
	if sys.argv[2] != "--show-asm":
		tos = sys.argv[2]
	else:
		showasm = True
		tos = sys.argv[1]+".out"


f = open(sys.argv[1], "r")
r = []
for l in f:
	s = l.split(' ')
	if s[0] == "#operation":
		name = s[1]
		i = s[2].rstrip()
		GLOBALNAMES.append(name)
		OPERATIONS[name] = int(i)
	elif s[0] == "#type":
		name = s[1]
		i = s[2].rstrip()
		TYPES[name] = int(i)
	elif len(stripline(l)) > 0 and stripline(l)[0] == "#":
		pass
	else:
		r.append(l.rstrip())



parsed = parse(r)
preasm = precomp(parsed)
bytecode = comp(preasm)

asm = add_io()
asm += add_str()
asm += ass(bytecode)

save(asm, tos)

if showasm:
	print "PARSED"
	pp.pprint(parsed)
	print "PRECOMPILED"
	pp.pprint(preasm)
	print "BYTECODE"
	pp.pprint(bytecode)
	print "ASSEMBLED"
	pp.pprint(asm)
	print "STRINGS"
	print STRINGS


