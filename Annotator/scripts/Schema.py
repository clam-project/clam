# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import sys
from xml.etree import ElementTree as ET
from copy import deepcopy
from io import StringIO

_StandardException = Exception

class Schema :
	class Exception(_StandardException) :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	def __init__(self, file=None) :
		if file is None:
			file = StringIO("<DescriptionScheme><Attributes/></DescriptionScheme>")
		self.tree = ET.parse(file)
		self.doc = self.tree.getroot()

	def Dump(self, file=sys.stdout):
		ET.indent(self.tree, space='\t')
		self.tree.write(file, xml_declaration=True, encoding='unicode')
		file.flush()

	def SelectAttribute(self, scope, name) :
		return self.doc.findall('.//Attribute[@scope="%s"][@name="%s"]' % (scope, name))

	def InsertAttribute(self, source, scope, name, targetScope, targetName=None) :
		nodesToMove = source.SelectAttribute(scope,name)
		if len(nodesToMove)!=1 :
			raise Schema.Exception("Attribute '"+scope+"::"+name+"' not found")
		nodeToMove  = nodesToMove[0]
		newAttribute = deepcopy(nodeToMove)
		newAttribute.set("scope", targetScope)
		if targetName != None : 
			newAttribute.set("name", targetName)
		self.doc.findall('.//Attributes')[0].append(newAttribute)

	def RemoveAttribute(self, scope, name) :
		nodesToRemove = self.SelectAttribute(scope,name)
		if len(nodesToRemove)!=1 :
			raise Schema.Exception("Attribute '"+scope+"::"+name+"' not found")
		self.doc.findall('.//Attributes')[0].remove(nodesToRemove[0])

	# TODO: Test this method
	def TranslateChildScope(self, scope, name, scopeMapping) :
		attributes = self.SelectAttribute(scope,name)
		for childScope in attributes[0].findall('ChildScope') :
			try :
				childScope.text = scopeMapping[childScope.text]
			except KeyError:
				# TODO: Remove the child scope as it doesn't exist in the new context
				pass



