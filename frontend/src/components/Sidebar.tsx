import { useState } from "react";
import './Sidebar.css'

interface Page {
  id: string
  icon: string
  label: string
  href?: string
}

interface Props {
  username: string
  onLogout: () => void
}

const pages: Page[] = [
  { id: 'home',     icon: '🏠', label: 'home' },
  { id: 'inbox',    icon: '📥', label: 'inbox' },
  { id: 'search',   icon: '🔍', label: 'search' },
  { id: 'settings', icon: '⚙️', label: 'settings' },
]

const favorites: Page[] = [
  { id: 'notes',    icon: '📝', label: 'notes' },
  { id: 'tasks',    icon: '✅', label: 'tasks' },
  { id: 'projects', icon: '📁', label: 'projects' },
  { id: 'docs',     icon: '📄', label: 'docs' },
]

export default function Sidebar({ username, onLogout }: Props) {
  const [active, setActive]       = useState('home')
  const [collapsed, setCollapsed] = useState(false)
  const [favOpen, setFavOpen]     = useState(true)

  return (
    <aside className={`sidebar ${collapsed ? 'collapsed' : ''}`}>

      {/*workspace*/}
      <div className="sidebar-workspace">
        <div className="workspace-avatar">
          {username[0].toUpperCase()}
        </div>
        {!collapsed && (
          <div className="workspace-info">
            <span className="workspace-name">{username} workspace</span>
            <span className="workspace-plan">Free Plan</span>
          </div>
        )}
        <button className="collapse-btn" onClick={() => setCollapsed(!collapsed)}>
          {collapsed ? '›' : '‹'}
        </button>
      </div>

      {/* Main Nav */}
      <nav className="sidebar-nav">
        {pages.map(p => (
          <button
            key={p.id}
            className={`nav-item ${active === p.id ? 'active' : ''}`}
            onClick={() => setActive(p.id)}
          >
            <span className="nav-icons">{p.icon}</span>
            {!collapsed && <span className="nav-label">{p.label}</span>}
          </button>
        ))}
      </nav>

      <div className="sidebar-divider" />

      {/* My Page */}
      {!collapsed && (
        <div className="sidebar-section">
          <button
            className="section-header"
            onClick={() => setFavOpen(!favOpen)}
          >
            <span>{favOpen ? '▾' : '▸'}</span>
            <span>My Page</span>
          </button>

          {favOpen && (
            <div className="section-items">
              {favorites.map(p => (
                <button
                  key={p.id}
                  className={`nav-item ${active === p.id ? 'active' : ''}`}
                  onClick={() => setActive(p.id)}
                >
                  <span className="nav-icon">{p.icon}</span>
                  <span className="nav-label">{p.label}</span>
                </button>
              ))}

              {/* New Page */}
              <button className="nav-item add-page">
                <span className="'nav-icon">＋</span>
                <span className="nav-label">New Page</span>
              </button>
            </div>
          )}
        </div>
      )}

      {/* Logout */}
      <div className="sidebar-footer">
        <button className="nav-item logout-btn" onClick={onLogout}>
          <span className="nav-icon">🚪</span>
          {!collapsed && <span className="nav-label">Log out</span>}
        </button>
      </div>
      
    </aside>
  )
}