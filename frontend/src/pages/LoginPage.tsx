import React, { useState } from 'react'

interface Props {
  onLogin: (token: string) => void
}

export default function LoginPage({ onLogin }: Props) {
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')
  const [error, setError]       = useState('')
  const [loading, setLoading]   = useState(false)

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault()
    setError('')
    setLoading(true)

    try {
      const res = await fetch('/api/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username, password }),
      })

      const data = await res.json()

      if (data.success) {
        onLogin(data.token)
      } else {
        setError(data.message || 'Login Error')
      }
    } catch (err) {
      setError('Unable to connect to the server')
    } finally {
      setLoading(false)
    }
  }

  return (
    <div style={{
      inset: 0,
      background: '#0a0a0f',
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
      fontFamily: 'system-ui, sans-serif',
      position: 'fixed',
      overflow: 'hidden',
    }}>
      <svg style={{ position: 'absolute', inset: 0, width: '100%', height: '100%', pointerEvents: 'none' }}>
        {/* 左上 */}
        <line x1="0" y1="80" x2="120" y2="80" stroke="#1e2030" strokeWidth="1"/>
        <line x1="120" y1="80" x2="120" y2="100" stroke="#1e2030" strokeWidth="1"/>
        <circle cx="120" cy="80" r="3" fill="none" stroke="#3a3f5c" strokeWidth="1"/>
        <rect x="10" y="65" width="80" height="30" rx="4" fill="none" stroke="#1e2030" strokeWidth="1"/>
        {/* 右上 */}
        <line x1="100%" y1="80" x2="calc(100% - 120px)" y2="80" stroke="#1e2030" strokeWidth="1"/>
        <circle cx="calc(100% - 120px)" cy="80" r="3" fill="none" stroke="#3a3f5c" strokeWidth="1"/>
        <rect x="calc(100% - 90px)" y="65" width="80" height="30" rx="4" fill="none" stroke="#1e2030" strokeWidth="1"/>
        {/* 左下 */}
        <line x1="0" y1="calc(100% - 80px)" x2="120" y2="calc(100% - 80px)" stroke="#1e2030" strokeWidth="1"/>
        <circle cx="120" cy="calc(100% - 80px)" r="3" fill="none" stroke="#3a3f5c" strokeWidth="1"/>
        <rect x="10" y="calc(100% - 95px)" width="80" height="30" rx="4" fill="none" stroke="#1e2030" strokeWidth="1"/>
        {/* 右下 */}
        <line x1="100%" y1="calc(100% - 80px)" x2="calc(100% - 120px)" y2="calc(100% - 80px)" stroke="#1e2030" strokeWidth="1"/>
        <circle cx="calc(100% - 120px)" cy="calc(100% - 80px)" r="3" fill="none" stroke="#3a3f5c" strokeWidth="1"/>
        <rect x="calc(100% - 90px)" y="calc(100% - 95px)" width="80" height="30" rx="4" fill="none" stroke="#1e2030" strokeWidth="1"/>
      </svg>

      {/*Login Card*/}
      <div style={{
        width: '100%',
        maxWidth: 340,
        background: '#111118',
        border: '1px solid #1e2030',
        borderRadius: 16,
        padding: '2rem 1.75rem',
        position: 'relative',
        zIndex: 1,
      }}>
        {/* Logo */}
        <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'center', gap: 12, marginBottom: '1.5rem' }}>
          <div style={{ display: 'flex', gap: 4, opacity: 0.3 }}>
            {[0,1,2].map(i => <span key={i} style={{ display: 'block', width: 6, height: 6, borderRadius: 1, background: '#4a5080' }}/>)}
          </div>
          <div style={{
            width: 40, height: 40, borderRadius: '50%',
            border: '1.5px solid #3a4070',
            display: 'flex', alignItems: 'center', justifyContent: 'center',
          }}>
            <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="#e0e0ff" strokeWidth="1.5">
              <polyline points="4 17 10 11 4 5"/>
              <line x1="12" y1="19" x2="20" y2="19"/>
            </svg>
          </div>
          <div style={{ display: 'flex', gap: 4, opacity: 0.3 }}>
            {[0,1,2].map(i => <span key={i} style={{ display: 'block', width: 6, height: 6, borderRadius: 1, background: '#4a5080' }}/>)}
          </div>
        </div>

        {/* Title */}
        <h2 style={{ textAlign: 'center', fontSize: 20, fontWeight: 500, color: '#e8e8ff', margin: '0 0 1.5rem' }}>
          Welcome back
        </h2>
        <p style={{ textAlign: 'center', fontSize: 13, color: '#5a5f80', margin: '0 0 1.5rem' }}>
          Don't have an account?{' '}
          <a href="#" style={{ color: '#6b7fff', textDecoration: 'none', fontWeight: 500}}>Sign up</a>
        </p>

        {/* Error Message */}
        {error && (
          <p style={{ color: '#ff6b6b', fontSize: 13, textAlign: 'center', margin: '0 0 1rem' }}>
            {error}
          </p>
        )}

        <form onSubmit={handleSubmit}>
          {/* account */}
          <div style={{
            display: 'flex', alignItems: 'center', gap: 10,
            background: '#0d0e18', border: '1px solid #1e2030',
            borderRadius: 8, padding: '10px 14px', marginBottom: 12,
          }}>
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="#3a4070" strokeWidth="1.5" style={{ flexShrink: 0 }}>
              <rect x="2" y="4" width="20" height="16" rx="2"/>
              <path d="m22 7-8.97 5.7a1.94 1.94 0 0 1-2.06 0L2 7"/>
            </svg>
            <input
              type="text"
              placeholder="username"
              value={username}
              onChange={e => setUsername(e.target.value)}
              required
              style={{
                background: 'transparent', border: 'none', outline: 'none',
                fontSize: 14, color: '#9090c0', width: '100%', fontFamily: 'inherit',
              }}
            />
          </div>

          <div style={{
            display: 'flex', alignItems: 'center', gap: 10,
            background: '#0d0e18', border: '1px solid #1e2030',
            borderRadius: 8, padding: '10px 14px', marginBottom: '1.25rem',
          }}>
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="#3a4070" strokeWidth="1.5" style={{ flexShrink: 0 }}>
              <rect x="3" y="11" width="18" height="11" rx="2"/>
              <path d="M7 11V7a5 5 0 0 1 10 0v4"/>
            </svg>
            <input
              type="password"
              placeholder="Password"
              value={password}
              onChange={e => setPassword(e.target.value)}
              required
              style={{
                background: 'transparent', border: 'none', outline: 'none',
                fontSize: 14, color: '#9090c0', width: '100%', fontFamily: 'inherit',
              }}
            />
          </div>

          {/* button */}
          <button
            type="submit"
            disabled={loading}
            style={{
              width: '100%', padding: 11,
              background: loading ? '#2a3db0' : '#3d5afe',
              border: 'none', borderRadius: 8,
              color: '#fff', fontSize: 14, fontWeight: 500,
              cursor: loading ? 'not-allowed' : 'pointer',
              fontFamily: 'inherit', marginBottom: '1.25rem',
              transition: 'background 0.2s',
            }}
          >
            {loading ? 'Logging in...' : 'Login'}
          </button>
        </form>
      </div>
    </div>
  )
}