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


from xml.etree import ElementTree as ET
from copy import deepcopy
import sys
from io import StringIO

class Pool :
	class Exception :
		def __init__(self, message) :
			self.what = message
		def __str__(self) :
			return self.what

	def __init__(self, file=None, domnodelist=None) :
		if file is None:
			file = StringIO("<DescriptorsPool/>")

		self.tree = ET.parse(file)
		self.doc = self.tree.getroot()

		if domnodelist is not None:
			for node in domnodelist:
				cloned = deepcopy(node)
				self.doc.append(cloned)

	def Dump(self, file=sys.stdout):
		ET.indent(self.tree, space='\t')
		self.tree.write(file, xml_declaration=True, encoding='unicode')
		file.flush()

	def GetScopeSize(self, scope) :
		for node in self.doc.findall('.//ScopePool[@name="%s"]' % scope) :
			return int(node.get('size'))
		raise Pool.Exception("Scope '" + scope + "' not found")

	def SelectAttributes(self, scope, name) :
		return self.doc.findall('.//ScopePool[@name="%s"]/AttributePool[@name="%s"]' % (scope, name))

	def SelectScope(self, scope) :
		return self.doc.findall('.//ScopePool[@name="%s"]' % scope)

	def RemoveAttribute(self, scope, name) :
		location = self.SelectScope(scope)
		if len(location)!=1:
			raise Pool.Exception("Scope '"+scope+"' not found while removing '"+ 
				scope+"::"+name+"'")
		toRemove = self.SelectAttributes(scope, name)
		if len(toRemove)!=1:
			raise Pool.Exception("Attribute '"+name+"' not found while removing '"+ 
				scope+"::"+name+"'")
		location[0].remove(toRemove[0])
		

	def InsertAttribute(self, source, scope, name, targetScope, targetName=None) :
		if targetName == None : targetName = name
		nodesToMove = source.SelectAttributes(scope,name)
		if len(nodesToMove)<1 :
			raise Pool.Exception("Attribute '"+scope+"::"+name+"' not found")
		sourceSize = source.GetScopeSize(scope)
		self.AssureScopeWithPopulation(targetScope, sourceSize)
		try :
			self.RemoveAttribute(targetScope, targetName)
		except : pass # if it does not exist that's ok
		self._InsertNode(nodesToMove, targetScope, targetName)

	def _InsertNode(self, nodesToMove, scope, name = None) :
		scopeNodes = self.doc.findall('.//ScopePool[@name="%s"]' % scope)
		for scopeNode in scopeNodes :
			for node in nodesToMove :
				cloned = deepcopy(node)
				if name != None :
					cloned.set("name", name)
				scopeNode.append(cloned)
				return
		assert None, "Target scope not found"

	def PopulateScope(self, scope, population) :
		newScope = ET.SubElement(self.doc, "ScopePool")
		newScope.set("name", scope)
		newScope.set("size", str(population))

	def AssureScopeWithPopulation(self, scope, population) :
		try :
			actualSize = self.GetScopeSize(scope)
		except Pool.Exception :
			self.PopulateScope(scope, population)
			return
		if actualSize==population : return
		raise Pool.Exception(
			"Requested size for scope '"+ scope +
			"' was " + str(population) +
			" but it is actually " + str(actualSize))
	def PresentAttributes(self) :
		ids=[]
		for scopeNode in self.doc.findall('.//ScopePool') :
			scopeName = scopeNode.get('name')
			for attributeNode in scopeNode.findall('AttributePool') :
				ids.append(str(scopeName + "::" + attributeNode.get('name')))
		return ids


	

