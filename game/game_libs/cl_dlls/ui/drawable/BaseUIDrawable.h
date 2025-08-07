#pragma once

class CBaseUIDrawable
{
public:
	virtual ~CBaseUIDrawable()
	{
	}

	virtual int Init()
	{
		return 0;
	}

	virtual int VidInit()
	{
		return 0;
	}

	virtual int Draw(float)
	{
		return 0;
	}

	virtual void Think()
	{
		return;
	}

	virtual void Reset()
	{
		return;
	}

	// Called every time a server is connected to
	virtual void InitHUDData()
	{
	}
};
