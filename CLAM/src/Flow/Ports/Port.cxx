/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Port.hxx"

namespace CLAM {

	// function to check the connection of two ports

	bool PortsAreConnected(CLAM::Port &p1, CLAM::Port &p2)
	{
		if (dynamic_cast<InPort*>(&p1) != NULL) // p1 is inport
		{
			// if p2 is inport, assert
			CLAM_ASSERT(dynamic_cast<InPort*>(&p2) == NULL,
				    "Error, the two ports introduced are the same type");
		}
		if (dynamic_cast<OutPort*>(&p1) != NULL) // p1 is inport
		{
			// if p2 is inport, assert
			CLAM_ASSERT(dynamic_cast<OutPort*>(&p2) == NULL,
				    "Error, the two ports introduced are the same type");
		}
		if (p1.GetProcessingData() == p2.GetProcessingData())
		{
			return p1.IsAttached(); // if both aren't attached, it mustn't be true
		}

		return false;
	}



	// Data visitor

	void DataVisitor::ErrMsg(std::string &msg,const std::string &data_class)
	{
		msg=  "No Visit method implemented in visitor ";
		msg+= ClassName() + " for the ";
		msg+= data_class + " data class\n";
	}

	void DataVisitor::Visit(ProcessingData& data)
	{
		throw Err("DataVisitor::Visit(): Visitor not implemented for the data class\n");
	}
	void DataVisitor::Visit(Spectrum& data)
	{
		std::string msg;
		ErrMsg(msg,"Spectrum");
		throw(Err(msg.c_str()));
	}
	void DataVisitor::Visit(Audio& data)
	{
		std::string msg;
		ErrMsg(msg,"Audio");
		throw(Err(msg.c_str()));
	}
	void DataVisitor::Visit(Peak& data)
	{
		std::string msg;
		ErrMsg(msg,"Peak");
		throw(Err(msg.c_str()));
	}
	void DataVisitor::Visit(Envelope& data)
	{
		std::string msg;
		ErrMsg(msg,"Envelope");
		throw(Err(msg.c_str()));
	}


	// InPortTmpls

	InPort::InPort(const std::string &n,
				   Processing *o,
				   int length,
				   int hop,
				   bool inplace) 
		: Port(n,o,length,hop),
		  mCanDoInplace(inplace)

	{}

	// OutPortTmpls

	OutPort::OutPort(const std::string &n,
					 Processing *o,
					 int length,
					 int hop )
		: Port(n,o,length,hop)
	{}


}
