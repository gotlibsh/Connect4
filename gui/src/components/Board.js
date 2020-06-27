import React, { Component } from 'react'
import Cube from './Cube'
import { tableStyle } from '../style'

const player = {
    first: 'red',
    second: 'yellow'
}

export default class Board extends Component {

    constructor(props) {
        super(props)

        this.myName = props.startingPlayer === 'human' ? player.first : player.second
        this.computerName = props.startingPlayer === 'machine' ? player.first : player.second
        this.isGameOver = false

        this.state = this.clearState()
    }

    clearState = () => {
        return {
            table: this.getEmptyTable(),
            newest: { row: null, col: null },
            turn: player.first,
            res: null,
            columnOver: null,
            winningSeq: []
        }
    }

    componentDidMount = () => {
        if (this.props.startingPlayer === 'machine') {
            this.fetchFromServer()
        }
    }

    switchTurns = () => {
        this.setState(prevState => {
            return { turn: prevState.turn === this.myName ? this.computerName : this.myName }
        })
    }

    getEmptyTable = () => {
        let table = new Array(this.props.rows)

        for (let i = 0; i < this.props.rows; i++) {
            table[i] = new Array(this.props.cols)
            table[i].fill(null)
        }

        return table
    }

    handleMouseClick = (colIndex) => {
        if (this.state.turn === this.myName && !this.isGameOver) {
            this.handleMove(colIndex, this.fetchFromServer)
        }
    }

    handleMove = (colIndex, callback) => {
        let rowIndex = this.getDeepestIndex(colIndex)
            
        if (rowIndex >= 0) {
            this.insertElement(rowIndex, colIndex, this.state.turn, callback)
            this.switchTurns()
        }
    }

    fetchFromServer = () => {
        this.props.serverCall(this.state.table, this.computerName, (res) => this.setState({ res }, () => this.handleResult()))
    }

    findWinningSeq = (winner) => {
        let table = this.state.table

        for (let row = 0; row < this.props.rows; row++) {
            for (let col = 0; col <= this.props.cols - 4; col++) {
                if (table[row][col] === winner && table[row][col + 1] === winner && table[row][col + 2] === winner && table[row][col + 3] === winner) {
                    this.setState({ winningSeq: [{row, col}, {row, col: col + 1}, {row, col: col + 2}, {row, col: col + 3}]})
                    return
                }
            }
        }

        for (let row = 0; row <= this.props.rows - 4; row++) {
            for (let col = 0; col < this.props.cols; col++) {
                if (table[row][col] === winner && table[row + 1][col] === winner && table[row + 2][col] === winner && table[row + 3][col] === winner) {
                    this.setState({ winningSeq: [{row, col}, {row: row + 1, col}, {row: row + 2, col}, {row: row + 3, col}]})
                    return
                }
            }
        }

        for (let row = 0; row <= this.props.rows - 4; row++) {
            for (let col = 0; col <= this.props.cols - 4; col++) {
                if (table[row][col] === winner && table[row + 1][col + 1] === winner && table[row + 2][col + 2] === winner && table[row + 3][col + 3] === winner) {
                    this.setState({ winningSeq: [{row, col}, {row: row + 1, col: col + 1}, {row: row + 2, col: col + 2}, {row: row + 3, col: col + 3}]})
                    return
                }
            }
        }

        for (let row = 0; row <= this.props.rows - 4; row++) {
            for (let col = this.props.cols - 4; col < this.props.cols; col++) {
                if (table[row][col] === winner && table[row + 1][col - 1] === winner && table[row + 2][col - 2] === winner && table[row + 3][col - 3] === winner) {
                    this.setState({ winningSeq: [{row, col}, {row: row + 1, col: col - 1}, {row: row + 2, col: col - 2}, {row: row + 3, col: col - 3}]})
                    return
                }
            }
        }
    }

    handleResult = () => {
        if (this.state.res.column !== null) {
            this.handleMove(this.state.res.column - 1, () => null)
        }
        if (this.state.res.is_game_over) {
            this.isGameOver = true
        }
    }

    insertElement(row, col, val, callback) {
        this.setState((prevState) => {
            let newTable = prevState.table
            newTable[row][col] = val
            return { table: newTable, newest: { row, col } }
        }, () => {
            callback()
            if (this.isGameOver) {
                this.findWinningSeq(this.state.res.winner)
            }
        })
    }

    getDeepestIndex = (colIndex) => {
        for (let rowIndex in this.state.table)
            if (this.state.table[rowIndex][colIndex] !== null)
                return rowIndex - 1

        return this.state.table.length - 1
    }

    handleMouseOver = (columnOver) => {
        this.setState({ columnOver })
    }

    isNewest = (i, j) => i === this.state.newest.row && j === this.state.newest.col

    getBackgroundColor = (row, col) => {
        return {
            backgroundColor: this.state.winningSeq.some(pair => pair.row === row && pair.col === col) ? 'silver' : 'white'
        }
    }

    render = () =>
        <div style={tableStyle.table}>
            <table onMouseOut={() => this.setState({ columnOver: null })}>
                <tbody>
                    {this.state.table.map((row, i) => 
                        <tr style={tableStyle.row} key={i}>
                            {row.map((col, j) => <td onMouseOver={() => this.handleMouseOver(j)} onClick={() => this.handleMouseClick(j)} 
                            style={{...tableStyle.col, border: this.state.columnOver === j ? 'solid #007bff 2px' : 'solid black 1px', ...this.getBackgroundColor(i, j)}} key={j}>{col ? 
                                <Cube value={col} animate={this.isNewest(i, j)}/> : null}</td>)}
                        </tr>
                    )}
                </tbody>
            </table>
        </div>    
}