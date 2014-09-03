<?php

//AXXX
class TMGMTBulkJobItem {
  var $plugin;
  var $item_type;
  var $item_id;
  function __construct($plugin, $item_type, $item_id) {
    $this->plugin = $plugin;
    $this->item_type = $item_type;
    $this->item_id = $item_id;
  }
}
