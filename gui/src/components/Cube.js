import React from 'react'
import { cubeStyle, colors } from '../style'
import { GiPlainCircle } from 'react-icons/gi'

const Cube = (props) => 
    <div style={cubeStyle.wrapper}>
        <GiPlainCircle style={{ ...cubeStyle.icon, color: colors[props.value], border: props.animate ? '2px dotted #007bff' : '', borderRadius: '50px' }}/>
    </div>

export default Cube
