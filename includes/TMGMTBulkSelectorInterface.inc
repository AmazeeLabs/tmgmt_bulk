<?php

//AXXX
interface TMGMTBulkSelectorInterface {

  /**
   * AXXX
   *
   * @param array $item_types
   * @param string $translated_to
   * @param string $not_exists_in_jobs
   * @param array $specific
   *
   * @return TMGMTBulkJobItem[]
   */
  public function searchItems(array $item_types, $translated_to, $not_exists_in_jobs, array $specific);

  //AXXX
  public function getFormSpecificFilters();
}
