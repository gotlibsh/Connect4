import React from 'react';
import Select from 'react-select'
import { Radio, RadioGroup, FormControlLabel, FormControl, FormLabel } from '@material-ui/core';

const options = [
    { label: 'Easy', value: 1 },
    { label: 'Medium', value: 2 },
    { label: 'Hard', value: 10 }
  ]

const Form = (props) => 
    <>
        <FormControl component="fieldset">
          <FormLabel component="legend">Starting player</FormLabel>
          <RadioGroup value={props.startingPlayer} onChange={(event) => props.setStartingPlayer(event.target.value)}>
            <FormControlLabel value="human" control={<Radio color='primary' />} label="Human" />
            <FormControlLabel value="machine" control={<Radio color='primary' />} label="Machine" />
          </RadioGroup>
        </FormControl>
        <Select options={options} defaultValue={options[0]} onChange={(option) => props.setDepth(option.value)} />
    </>

export default Form