import React from 'react';

const handleOverflow = (value) => {
    if (value > 100) return 100
    if (value < 0) return 0
    return value
}

const EvaluationBar = (props) => {
    return (
        <div style={{width: '40px', height: '300px'}}>
            <div style={{width: '100%', height: `${handleOverflow(100 - props.red)}%`, backgroundColor: 'gold', transition: 'all .5s'}}></div>
            <div style={{width: '100%', height: `${handleOverflow(props.red)}%`, backgroundColor: 'red', transition: 'all .5s'}}></div>
        </div>
    )
}

export default EvaluationBar