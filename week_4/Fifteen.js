const fs = require('fs')
class WordFrequencyFramework{
  _load_event_handlers = []
  _dowork_event_handlers=[]
  _end_event_handlers = []

  register_for_load_event(handler){
    this._load_event_handlers.push(handler);
  }
  register_for_dowork_event(handler){
    this._dowork_event_handlers.push(handler);
  }
  register_for_end_event(handler){
    this._end_event_handlers.push(handler);
  }
  run(file_path){
    this._load_event_handlers.forEach(h => {
      h(file_path)
});
    this._dowork_event_handlers.forEach(h => {
      h()
});
    this._end_event_handlers.forEach(h => {
      h()
});
  }
}

class StopwordFilter{
  _stop_words = []
  constructor(wfapp){
    wfapp.register_for_load_event(this.__load.bind(this))
  }
  __load(){
    const stopString = fs.readFileSync('../stop_words.txt')
    this._stop_words = stopString.toString().split(',');    
  }
  is_stop_words(word){
    return this._stop_words.includes(word);
  }
}

class DataStorage{
  _word_event_handler =[]
  _data = []
  constructor(wfapp,stop_word_filter){
    this._stop_word_filter = stop_word_filter;
    wfapp.register_for_load_event(this.__load.bind(this))
    wfapp.register_for_dowork_event(this.__produce_words.bind(this))
  }
  __load(file_path){
    var inputD = fs.readFileSync(file_path);
    var data = inputD.toString().replace(/[^a-z0-9]/gi, ' ');
    data = data.toLowerCase();
    this.data = data.split(' ');
  }
  __produce_words(){
    this.data.forEach(w => {
          if ((! this._stop_word_filter.is_stop_words(w))&&(w.length>1)){
            this._word_event_handler.forEach(h => {h(w)});
          }
    });
  }
  register_for_word_event(handler){
    this._word_event_handler.push(handler)
  }
  
}
class WordFrequencyCounter{
  fq = {}
  constructor(wfapp, data_storage){
        data_storage.register_for_word_event(this.__increment_count.bind(this))
        wfapp.register_for_end_event(this.__print_freqs.bind(this))
  }

  __increment_count(word){
    if (word in this.fq){
      this.fq[word] +=1 ;}
    else{
      this.fq[word] = 1;
    } 
  }
  __print_freqs(){
   let rawCounts = []
    for (let [word, frequency] of Object.entries(this.fq)) {
        rawCounts.push({word, frequency})
    }
    var sorted_list =  rawCounts.sort(function(first, second) {
        return second.frequency - first.frequency;
    });
    for (let i = 0; i < Math.min(25,sorted_list.length); i++) {
        console.log(sorted_list[i].word, ' - ', sorted_list[i].frequency)
    }       
  }
}


let wfapp = new WordFrequencyFramework();
let stop_word_filter = new StopwordFilter(wfapp);
let data_storage = new DataStorage(wfapp, stop_word_filter)
let word_freq_cnt = new WordFrequencyCounter(wfapp,data_storage)
wfapp.run(process.argv[2])

