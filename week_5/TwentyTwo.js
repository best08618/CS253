const fs = require('fs')
var assert = require('assert');

function extract_words(obj,file_path) {
  assert(typeof(file_path)=='string',"file_path: should be string!")
  assert(file_path,"file_path :Need none-empty string!")
  try{
  inputD = fs.readFileSync(file_path);
  }catch(err){
    console.log('I/O Error during reading',file_path)
    throw err;
  }
  data = inputD.toString().replace(/[^a-z0-9]/gi, ' ');
  data = data.toLowerCase();
  obj['data'] = data.split(' ');
}

function load_stop_words(obj){
  try{
  stopString = fs.readFileSync('../stop_words.txt')
  }catch(err){
    console.log('I/O error during reading stopword file');
    console.log(err);   
    throw err;
  }
  obj['stopwords'] = stopString.toString().split(',');
}

function increment_count(obj,word){
  assert(typeof(word)=='string',"word: should be string! ")
  assert(typeof obj['fq'] ==='object',"FQ : need to be dict!")
  if (word in obj['fq']){
  obj['fq'][word] +=1 ;}
  else{
    obj['fq'][word] = 1;
  }
}

let data_storage_obj = {
  data: [],
  init: function (file_path){
    extract_words(this,file_path);
  },
  words:function (){return this.data;}
};

let stop_words_obj = {
  stopwords: [],
  init: function (){
    load_stop_words(stop_words_obj);
  },
  'is_stop_word':function (word){ 
    assert(typeof(word) =='string','word: should be string!')
    return stop_words_obj.stopwords.includes(word);}
};

let word_freqs_obj = {
  fq: {},
  increment_count(word){
     increment_count(word_freqs_obj,word) 
  },
  sorted: function(){
    assert(Object.keys(this.fq).length > 0 ,'None empty fq is needed!')
    let rawCounts = []
    for (let [word, frequency] of Object.entries(this.fq)) {
        rawCounts.push({word, frequency})
    }
    return rawCounts.sort(function(first, second) {
        return second.frequency - first.frequency;
    });
    
  }
  
}

data_storage_obj['init'](process.argv[2]);
stop_words_obj['init']();

data_storage_obj['words']().forEach(item => {
  if ((! stop_words_obj['is_stop_word'](item)) && item.length > 1){
    word_freqs_obj['increment_count'](item)
  }
});

fq_result = word_freqs_obj['sorted']();
for (let i = 0; i < Math.min(25,fq_result.length); i++) {
    console.log(fq_result[i].word, ' - ', fq_result[i].frequency)
}





