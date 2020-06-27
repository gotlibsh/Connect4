import React from 'react'
import { Modal, Button } from 'react-bootstrap'

const MyModal = (props) => {
    return (
        <Modal show={props.showModal} animation={true} onHide={() => null} centered={props.centered}>
            <Modal.Header>
                <Modal.Title>{props.headline}</Modal.Title>
            </Modal.Header>
            {props.body ? 
            <Modal.Body>
                {props.body}
            </Modal.Body>
            : null}
            <Modal.Footer>
                {props.buttons.map((btn, i) => 
                    <Button variant="primary" onClick={() => btn.onClick()} key={i}>
                        {btn.text}
                    </Button>
                )}
            </Modal.Footer>
        </Modal>
    )
}

export default MyModal
