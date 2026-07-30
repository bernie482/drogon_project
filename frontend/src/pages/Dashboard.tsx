import { useState, useEffect } from 'react'
import Sidebar from '../components/Sidebar'

interface Props {
  token: string
  onLogout: () => void
}

function Dashboard({ token, onLogout }: Props) {
  const [user, setUser] = useState<{ username: string } | null>(null)

  useEffect(() => {
    console.log('token:', token)
    // Get User Info
    fetch('/api/me', {
        headers: { 'Authorization': `Bearer ${token}` }
    })
      .then(res => res.json())
      .then(data => {
        if (data.success) {
          setUser(data.user)
        } else {
          onLogout() // Token invalid
        }
      })
      .catch(() => onLogout())
  }, [token])

  const handleLogout = async () => {
    await fetch('/api/logout', {
      method: 'POST',
      headers: { 'Authorization': `Bearer ${token}` }
    })
    onLogout()
  }

  if (!user) return <p style={{ padding: 24 }}>Loading...</p>

  return (
    <div style={{ display: 'flex', height: '98%' }}>
      <Sidebar username={user.username} onLogout={handleLogout} />

      {/* Main Content */}
      <main style={{ flex: 1, padding: '40px 60px', background: '#fff' }}>
        <h1 style={{
          fontSize: 32,
          fontWeight: 700,
          color: '#37352f',
          marginBottom: 8,
          fontFamily: 'ui-sans-serif, sans-serif'
        }}>
          Welcome Back，{user.username}
        </h1>
        <p style={{ color: '#9b9a97', fontSize: 14 }}>
          What do you want to do today?
        </p>
      </main>
    </div>
  )
}

export default Dashboard