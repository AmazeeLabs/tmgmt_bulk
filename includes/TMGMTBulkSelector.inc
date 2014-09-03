<?php

//AXXX
class TMGMTBulkSelector {

  //AXXX
  protected function filterExistingInJobs($plugin, $item_type, $item_ids, $job_target_language) {
    $query = db_select('tmgmt_job_item', 'ji')
      ->condition('ji.plugin', $plugin)
      ->condition('ji.item_type', $item_type)
      ->condition('ji.item_id', $item_ids, 'IN')
      ->fields('ji', array('item_id'))
      ->distinct();
    $query->innerJoin(
      'tmgmt_job',
      'j',
      'j.tjid = ji.tjid AND j.target_language = :language',
      array(':language' => $job_target_language)
    );
    return array_diff($item_ids, $query->execute()->fetchCol());
  }

  //AXXX def
  public function getFormSpecificFilters() {
    return array();
  }
}
