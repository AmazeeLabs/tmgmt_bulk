<?php

//AXXX
interface TMGMTBulkSelectorInterface {

  /**
   * AXXX
   *
   * @param string $item_type
   * @param string $translated_to
   * @param string $not_exists_in_jobs
   * @param array $specific
   *
   * @return string[]
   */
  public function searchItems($item_type, $translated_to, $not_exists_in_jobs, array $specific);

  //AXXX
  public function getFormSpecificFilters();
}
