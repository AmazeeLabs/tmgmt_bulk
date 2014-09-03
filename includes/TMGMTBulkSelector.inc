<?php

//AXXX
class TMGMTBulkSelector {

  //AXXX
  protected function notExistsInJobs(SelectQueryInterface $query, $plugin, $item_type_field_alias, $item_id_field_alias, $job_target_language) {
    $job_item_table_alias = $query->leftJoin(
      'tmgmt_job_item',
      NULL,
      "%alias.plugin = :plugin AND %alias.item_type = $item_type_field_alias AND %alias.item_id = $item_id_field_alias",
      array(':plugin' => $plugin)
    );
    $job_table_alias = $query->leftJoin(
      'tmgmt_job',
      NULL,
      "%alias.tjid = $job_item_table_alias.tjid"
    );
    $query->condition(
      db_or()
        ->condition("$job_table_alias.target_language", $job_target_language, '<>')
        ->isNull("$job_table_alias.target_language")
    );
    $query->distinct();
  }

  //AXXX def
  public function getFormSpecificFilters() {
    return array();
  }
}
