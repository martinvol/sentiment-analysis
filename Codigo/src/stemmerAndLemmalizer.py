from nltk.stem.lancaster import LancasterStemmer
from nltk.stem import WordNetLemmatizer
import re
import unicodedata

def clean(s):
	"""
		Returns a cleaned, lowercased string
	"""
	out = " ".join(re.findall(r'\w+', s,flags = re.UNICODE | re.LOCALE)).lower()
	return out

archivo = open("../testData.tsv",'r')

lines = archivo.readlines()

lancaster_stemmer = LancasterStemmer()
lmtzr = WordNetLemmatizer()
newLines = []

for e, line in enumerate(lines):
	print e
	if not 0==e:
		r = line.strip().split("\t")
		id = r[0]
		review = ""
		label = ""
		if len(r) < 3 :
			review = r[1]
		else:
			label = r[1]
			review = r[2] 


		review = clean(review)
		review = lancaster_stemmer.stem(review) 
		review =  " ".join([lmtzr.lemmatize(i,'n') for i in review.split()])
		review =  " ".join([lmtzr.lemmatize(i,'v') for i in review.split()])
		try:
			review = unicodedata.normalize('NFKD', review).encode('ascii','ignore')
		except Exception, e:
			pass
		linea=[]
		if len(r) < 3 :
			linea=[id,review]
		else:
			linea=[id,label,review]
		newLines.append("\t".join(linea))
	else:
		newLines.append(line)

stringToWrite = "\n".join(x for x in newLines)
newArchivo = open("../testDataStemmedAndLemmalitized.tsv",'w')
newArchivo.write(stringToWrite)
newArchivo.close()