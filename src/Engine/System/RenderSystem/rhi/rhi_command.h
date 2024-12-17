#pragma once
#include "Structure/TB_List.hpp"
namespace TourBillon
{
	class RHICommandList;
	class RHICommand :public TBListMem 
	{
	public:
		virtual void ExecuteAndDestruct(RHICommandList& CmdList);
	};

	class RHICommandList :public TBList<RHICommand>
	{

	};
}