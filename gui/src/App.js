import React, { useState } from 'react';
import MyModal from './components/MyModal'
import Form from './components/Form'
import Board from './components/Board'
import EvaluationBar from './components/EvaluationBar'
import { appStyle } from './style'
import { Button } from 'react-bootstrap'
import 'bootstrap/dist/css/bootstrap.min.css';

const cols = 7
const rows = 6
const max = 40

const targetUrl = "http://127.0.0.1:5000/evaluate"

const requestOptions = {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' }
};

const App = () => {

  const [evaluation, setEvaluation] = useState(0)
  const [depth, setDepth] = useState(7)
  const [startingPlayer, setStartingPlayer] = useState('human')
  const [showMenuModal, setShowMenuModal] = useState(true);
  const [showGameOverModal, setShowGameOverModal] = useState(false);
  const [gameOverMessage, setGameOverMessage] = useState('')

  const serverCall = (board, turn, callback) => {
    fetch(targetUrl, { ...requestOptions, body: JSON.stringify({ board, turn, depth })})
      .then(res => res.json())
      .then(
        (result) => { 
          setEvaluation(result.eval)
          callback(result)
          if (result.is_game_over) {
            setGameOverMessage(result.winner)
            setShowGameOverModal(true)
          }
        },
        (error) => {
          alert('Error connecting to server')
        }
      )
  }

  const startGame = () => {
    setEvaluation(0)
    setShowMenuModal(true)
  }

  const modalContent = () => <Form setDepth={setDepth} setStartingPlayer={setStartingPlayer} startingPlayer={startingPlayer} />

  const menuBtns = [{ text: 'Play!', onClick: () => setShowMenuModal(false) }]
  const gameOverBtns = [{ text: 'Close', onClick: () => setShowGameOverModal(false) }]

  return (
    <div style={appStyle.app}>
      
      <MyModal 
        headline='Menu'
        showModal={showMenuModal} 
        body={modalContent()}
        buttons={menuBtns}
        centered
      />

      <MyModal 
        headline={<h1 style={{width: '100%', textAlign: 'center'}}>{gameOverMessage !== null ? gameOverMessage.toUpperCase() + ' WON!' : 'DRAW!'}</h1>}
        showModal={showGameOverModal} 
        buttons={gameOverBtns}
        centered
      />

      {!showMenuModal ? 
      <div style={{ display: 'flex', textAlign: 'center' }}>
        <div style={{marginLeft: '38%'}}>
          <div style={appStyle.score}>evaluation: {evaluation}</div>
          <Board 
            cols={cols} 
            rows={rows} 
            serverCall={serverCall} 
            startingPlayer={startingPlayer} 
            setEvaluation={setEvaluation}
          /> 
          <div style={{ marginTop: '30px' }}>
            <Button onClick={startGame}>New Game</Button>
          </div>
        </div>
        <div style={{ marginTop: '157px', marginLeft: '30px' }}>
          <EvaluationBar red={Math.floor(((evaluation + max) / (2 * max)) * 100)} />
        </div>
      </div>
        : null}
    </div>
  )
}

export default App;