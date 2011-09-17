#pragma once

class HWordToSpell
{
public:
	HWordToSpell();
	~HWordToSpell();
	inline void SetPart(CString strPart){m_strPart=strPart;}
	CString ParseWords(CString strWords);
protected:
	CString Word2Spell(CString strIn);
private:
	CString m_strPart;
};
