const fs = require('fs')
class TFQuarantine{
  constructor(func){
    this._funcs = [func]
  }
  bind(func){
    this._funcs.push(func)
    return this
  }
  execute(){
    function guard_callable(v) {
        if (typeof v === 'function') {
            return v();
        } else {
            return v;
        }
    }
    var value = () => null;
    this._funcs.forEach(func => {
      value = func(guard_callable(value))
      console.log(value)
    });
    console.log(guard_callable(value))
}         
}

function get_input(arg){
  function _f(){
    return process.argv[2]
  }
  return _f
}

function extract_words(file_path) {
  function _f(){
    inputD = fs.readFileSync(file_path);
    data = inputD.toString().replace(/[^a-z0-9]/gi, ' ');
    data = data.toLowerCase();
    return data.split(' ');
  }
  return _f 
}

function remove_stop_words(word_list){
  function _f(){
    words = [];
    stopString = fs.readFileSync('../stop_words.txt')
    stop_words =  stopString.toString().split(',');
    word_list.forEach(item => {
      if ((!stop_words.includes(item)) && item.length > 1){
        words.push(item)
      }
    });
    return words
  }
  return _f
}

function increment_count(word_list){
  fq = {}
  word_list.forEach(word=>{
  if (word in fq){
    fq[word] +=1 ;}
  else{
    fq[word] = 1;
  }
  });
  return fq
}

function sort(fq){
    let rawCounts = []
    for (let [word, frequency] of Object.entries(fq)) {
        rawCounts.push({word, frequency})
    }
    return rawCounts.sort(function(first, second) {
        return second.frequency - first.frequency;
    });
}

function top_25(word_freqs){
  top25 = ""
  for (let i = 0; i < Math.min(25,word_freqs.length); i++) {
      top25 += word_freqs[i].word + ' - ' + word_freqs[i].frequency + '\n'
  }
  return top25
}

let tf = new TFQuarantine(get_input)
tf.bind(extract_words).bind(remove_stop_words).bind(increment_count).bind(sort).bind(top_25).execute()





