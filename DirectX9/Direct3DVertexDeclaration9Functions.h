
D3D9Wrapper::IDirect3DVertexDeclaration9::IDirect3DVertexDeclaration9(D3D9Base::LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration)
    : D3D9Wrapper::IDirect3DUnknown((IUnknown*) pVertexDeclaration),
	pendingCreateVertexDeclaration(false),
	magic(0x7da43feb)
{
}

D3D9Wrapper::IDirect3DVertexDeclaration9* D3D9Wrapper::IDirect3DVertexDeclaration9::GetDirect3DVertexDeclaration9(D3D9Base::LPDIRECT3DVERTEXDECLARATION9 pVertexDeclaration)
{
    D3D9Wrapper::IDirect3DVertexDeclaration9* p = (D3D9Wrapper::IDirect3DVertexDeclaration9*) m_List.GetDataPtr(pVertexDeclaration);
    if (p == NULL)
    {
        p = new D3D9Wrapper::IDirect3DVertexDeclaration9(pVertexDeclaration);
		if (pVertexDeclaration) m_List.AddMember(pVertexDeclaration, p);
    }
    return p;
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3DVertexDeclaration9::AddRef(THIS)
{
	++m_ulRef;
	return m_pUnk->AddRef();
}

STDMETHODIMP_(ULONG) D3D9Wrapper::IDirect3DVertexDeclaration9::Release(THIS)
{
	LogDebug("IDirect3DVertexDeclaration9::Release handle=%x, counter=%d, this=%x\n", m_pUnk, m_ulRef, this);

	ULONG ulRef = m_pUnk ? m_pUnk->Release() : 0;
	LogDebug("  internal counter = %d\n", ulRef);

	--m_ulRef;

    if (ulRef == 0)
    {
		if (!LogDebug) LogInfo("IDirect3DVertexDeclaration9::Release handle=%x, counter=%d, internal counter = %d\n", m_pUnk, m_ulRef, ulRef);
		LogInfo("  deleting self\n");
		
        if (m_pUnk) m_List.DeleteMember(m_pUnk); 
		m_pUnk = 0;
        delete this;
        return 0L;
    }
    return ulRef;
}

STDMETHODIMP D3D9Wrapper::IDirect3DVertexDeclaration9::GetDevice(THIS_ IDirect3DDevice9** ppDevice)
{
	LogDebug("IDirect3DVertexDeclaration9::GetDevice called\n");

	CheckVertexDeclaration9(this);
	D3D9Base::IDirect3DDevice9 *origDevice;
	HRESULT hr = GetD3DVertexDeclaration9()->GetDevice(&origDevice);
	if (hr != S_OK)
	{
		LogInfo("  failed with hr = %x\n", hr);
		
		return hr;
	}
	D3D9Base::IDirect3DDevice9Ex *origDeviceEx;
	const IID IID_IDirect3DDevice9Ex = { 0xb18b10ce, 0x2649, 0x405a, { 0x87, 0xf, 0x95, 0xf7, 0x77, 0xd4, 0x31, 0x3a } };
	hr = origDevice->QueryInterface(IID_IDirect3DDevice9Ex, (void **) &origDeviceEx);
	origDevice->Release();
	if (hr != S_OK)
	{
		LogInfo("  failed IID_IDirect3DDevice9Ex cast with hr = %x\n", hr);
		
		return hr;
	}
	*ppDevice = D3D9Wrapper::IDirect3DDevice9::GetDirect3DDevice(origDeviceEx);
	return hr;
}

STDMETHODIMP D3D9Wrapper::IDirect3DVertexDeclaration9::GetDeclaration(THIS_ D3D9Base::D3DVERTEXELEMENT9* pElement,UINT* pNumElements)
{
	LogDebug("IDirect3DVertexDeclaration9::GetDeclaration called\n");

	CheckVertexDeclaration9(this);
	return GetD3DVertexDeclaration9()->GetDeclaration(pElement, pNumElements);
}
